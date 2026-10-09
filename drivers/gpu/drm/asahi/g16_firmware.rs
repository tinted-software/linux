// SPDX-License-Identifier: GPL-2.0-only OR MIT


#[cfg(test)]
#[path = "g16_resources.rs"]
mod g16_resources;
#[cfg(not(test))]
use crate::g16_resources;

const TEXT_SIZE: u64 = 0x60000;
const DATA_SIZE: u64 = 0x12c000;
pub(crate) const TEXT_SHA256: [u8; 32] = [
    0x3a, 0x6b, 0x8b, 0x5e, 0x83, 0x38, 0x4d, 0x5c, 0xc1, 0x92, 0x9d, 0x91, 0xb6, 0x36, 0x40, 0xfe,
    0xe8, 0xf5, 0x52, 0xb5, 0x10, 0x8c, 0x4e, 0xa6, 0x20, 0xbe, 0xdb, 0x7b, 0xbc, 0xa0, 0x31, 0x64,
];

/// The 25G83 G15G C0 image's identity without its per-Mac patchbay
/// (experimental J615 admission). The image-info header at __TEXT+0x200 names
/// the patchbay iBoot fills at load: 0x231 bytes of tagged values at 0x5ce4c.
/// Besides boot entropy (GKTS, ECAP) they hold this Mac's physical GPU
/// carveouts, which move with installed RAM and the iBoot layout, so the
/// J613's GKTS/ECAP-only digest above can only ever match a Mac laid out like
/// the one it was taken on. This digest is the same loaded J613 image with the
/// whole patchbay zeroed; every byte outside it, code and iBoot's GPU setup
/// records included, stays covered. The carveouts are not dropped: each is
/// compared with the region m1n1 reserved for it before the patchbay is zeroed.
#[cfg_attr(test, allow(dead_code))]
pub(crate) const PATCHBAY_TEXT_SHA256: [u8; 32] = [
    0x0f, 0xa7, 0x07, 0x99, 0x35, 0xab, 0xa3, 0x4a, 0x1f, 0xb9, 0xe3, 0x4b, 0x09, 0x08, 0xef, 0x98,
    0x46, 0x1f, 0x07, 0xe6, 0x9c, 0x71, 0x17, 0xd6, 0xa9, 0x3a, 0xb9, 0x25, 0x6c, 0x98, 0x5e, 0x78,
];
/// The same with iBoot's other writes zeroed too: header words 15 and 16 and
/// the two GPU setup records after the patchbay. What is left is byte for byte
/// the c000 __TEXT of Apple's 26.6.2 Firmware/agx/armfw_g15g.im4p, which the
/// J613 and the J615 share; only `asahi.g16_j615_setup_records=1` accepts it.
#[cfg_attr(test, allow(dead_code))]
pub(crate) const IMAGE_FILE_TEXT_SHA256: [u8; 32] = [
    0xc7, 0xec, 0x1a, 0xb3, 0x31, 0x8b, 0xe5, 0xa1, 0x0d, 0x4a, 0x2e, 0xeb, 0x5e, 0x25, 0x77, 0xbd,
    0xc3, 0xd4, 0xd1, 0xa5, 0xc0, 0xe9, 0x49, 0x06, 0xd3, 0xe5, 0x1b, 0xab, 0x22, 0x2e, 0xc1, 0x95,
];
/// iBoot-written outside the patchbay on the J613: header words 15-16 and
/// the setup records up to the next table (placeholders in the file).
const SETUP_WRITES: [core::ops::Range<usize>; 2] = [0x23c..0x244, 0x5d07d..0x5d630];

/// Zero iBoot's writes outside the patchbay (after `normalize_patchbay`).
fn normalize_setup_records(text: &mut [u8]) -> bool {
    if text.len() != TEXT_SIZE as usize {
        return false;
    }
    for range in SETUP_WRITES {
        text[range].fill(0);
    }
    true
}

const INFO_HEADER: usize = 0x200;
/// Header words 0-2 (branch, 'uuid', version 5) and 11-13 (patchbay offset,
/// patchbay size, text size) of the 25G83 C0 image; its UUID is words 5-8.
const INFO_WORDS: [(usize, u32); 6] = [(0, 0x1400021a), (1, 0x6469_7575), (2, 5),
    (11, 0x5ce4c), (12, 0x231), (13, 0x60000)];
/// 82C6019B-BE1A-3338-9DB9-30C20BBBCEB5, the iop-gfx-nub uuid on 26.6.2.
const IMAGE_UUID: [u8; 16] = [0x82, 0xc6, 0x01, 0x9b, 0xbe, 0x1a, 0x33, 0x38,
    0x9d, 0xb9, 0x30, 0xc2, 0x0b, 0xbb, 0xce, 0xb5];
const PATCHBAY: core::ops::Range<usize> = 0x5ce4c..0x5ce4c + 0x231;

fn le32(text: &[u8], offset: usize) -> Option<u32> {
    Some(u32::from_le_bytes(text.get(offset..offset + 4)?.try_into().ok()?))
}

/// Check the image-info header and the patchbay's carveout values against
/// this boot's reserved regions, then zero the patchbay. On any refusal the
/// copy is left as it was.
fn normalize_patchbay(text: &mut [u8], regions: &[(u64, u64); 4], firmware_va: u64) -> bool {
    if text.len() != TEXT_SIZE as usize
        || INFO_WORDS.iter().any(|&(i, v)| le32(text, INFO_HEADER + 4 * i) != Some(v))
        || text[INFO_HEADER + 0x14..INFO_HEADER + 0x24] != IMAGE_UUID
    {
        return false;
    }
    // Tag (as stored), the value it must hold, and whether it was seen once.
    let mut expected: [(&[u8; 4], u64, u32); 6] = [
        (b"BtpG", regions[0].0, 0), // ttbs
        (b"BPTP", regions[1].0, 0), // pagetables
        (b"SPTP", regions[1].1, 0), // pagetables size
        (b"BlpP", regions[2].0, 0), // handoff
        (b"S2xG", regions[3].0, 0), // shared-l2
        (b"BVTP", firmware_va, 0),  // firmware __TEXT VA
    ];
    let mut at = PATCHBAY.start;
    while at < PATCHBAY.end {
        let Some(length) = le32(text, at + 4) else { return false };
        let value = at + 8;
        let Some(end) = value.checked_add(length as usize) else { return false };
        if end > PATCHBAY.end {
            return false;
        }
        for item in expected.iter_mut() {
            if text[at..at + 4] == item.0[..] {
                let Ok(bytes) = <[u8; 8]>::try_from(&text[value..end]) else { return false };
                if u64::from_le_bytes(bytes) != item.1 {
                    return false;
                }
                item.2 += 1;
            }
        }
        at = end;
    }
    if at != PATCHBAY.end || expected.iter().any(|item| item.2 != 1) {
        return false;
    }
    text[PATCHBAY].fill(0);
    true
}

pub(crate) struct Image {
    pub(crate) text_size: u64,
    pub(crate) data_size: u64,
    pub(crate) text_sha256: [u8; 32],
    pub(crate) tlv_header: [u8; 8],
    pub(crate) gkts: usize,
    pub(crate) ecap: usize,
}

#[cfg_attr(not(test), allow(dead_code))]
const J613_IMAGE: Image = Image {
    text_size: TEXT_SIZE,
    data_size: DATA_SIZE,
    text_sha256: TEXT_SHA256,
    tlv_header: [0x4c, 0xce, 5, 0, 0x31, 2, 0, 0],
    gkts: 0x5ce4c,
    ecap: 0x5cf2f,
};

fn normalize_boot_entropy(text: &mut [u8], image: &Image) -> bool {
    let fields: [(usize, &[u8; 4]); 2] = [(image.gkts, b"GKTS"), (image.ecap, b"ECAP")];
    if text.len() != image.text_size as usize
        || text[0x22c..0x234] != image.tlv_header
        || fields.iter().any(|(offset, key)| {
            text[*offset..*offset + 4] != **key || text[*offset + 4..*offset + 8] != [8, 0, 0, 0]
        })
    {
        return false;
    }
    for (offset, _) in fields {
        text[offset + 8..offset + 16].fill(0);
    }
    true
}

#[derive(Debug)]
pub(crate) struct Firmware {
    pub(crate) resources: g16_resources::Resources,
    #[cfg(not(test))]
    pub(crate) board: &'static crate::g16_board::Board,
}

impl Firmware {
    pub(crate) const fn version(&self) -> &'static str {
        "RTKit-3255.160.4.release"
    }

    fn matching_layout(resources: &g16_resources::Resources, image: &Image) -> bool {
        resources.regions[4].size == image.text_size
            && resources.regions[5].size == image.data_size
            && resources.firmware_vas == [0xffff_fc00_0000_0000, 0xffff_fc00_0000_0000 + image.text_size]
    }

    #[cfg(test)]
    fn from_digest(resources: g16_resources::Resources, digest: [u8; 32]) -> Option<Self> {
        if !Self::matching_layout(&resources, &J613_IMAGE) || digest != J613_IMAGE.text_sha256 {
            return None;
        }
        Some(Self { resources })
    }
}

#[cfg(not(test))]
pub(crate) fn identify_loaded(
    pdev: &kernel::platform::Device<kernel::device::Core>,
    resources: g16_resources::Resources,
) -> kernel::error::Result<Firmware> {
    use kernel::{
        bindings, c_str,
        io::mem::{Mem, MemFlag},
        prelude::*,
    };

    let board = crate::g16_board::get()?;
    let f = &board.firmware;
    let image = Image { text_size: f.text_size, data_size: f.data_size, text_sha256: f.text_sha256,
        tlv_header: f.tlv_header, gkts: f.gkts, ecap: f.ecap };
    if !Firmware::matching_layout(&resources, &image) {
        dev_err!(pdev.as_ref(), "G16G: unsupported firmware segment layout\n");
        return Err(ENODEV);
    }
    let node = pdev.as_ref().of_node().ok_or(ENODEV)?;
    let text = node.reserved_mem_region_to_resource_byname(c_str!("fw-text"))?;
    if text.start() != resources.regions[4].base || text.size() != image.text_size {
        return Err(EINVAL);
    }
    // SAFETY: from_device checked the bootloader's no-map, nonoverlapping
    // firmware __TEXT region and its read-only segment flag. The mapping is
    // used only for hashing here and is dropped before returning. No mutable
    // reference, firmware patch, MMIO write or DMA is exposed by this function.
    let mapping = unsafe { Mem::try_new(text, MemFlag::WB.into()) }?;
    // SAFETY: the mapping covers the complete readable reserved __TEXT.
    let bytes = unsafe { core::slice::from_raw_parts(mapping.ptr(), mapping.size()) };
    let mut canonical = KVec::new();
    canonical.extend_from_slice(bytes, GFP_KERNEL)?;
    if !normalize_boot_entropy(&mut canonical, &image) {
        return Err(ENODEV);
    }
    if f.patchbay_identity {
        let r = &resources.regions;
        let regions = [(r[0].base, r[0].size), (r[1].base, r[1].size),
            (r[2].base, r[2].size), (r[3].base, r[3].size)];
        if !normalize_patchbay(&mut canonical, &regions, resources.firmware_vas[0]) {
            dev_err!(pdev.as_ref(),
                "G16G: {}: firmware image header or patchbay carveouts differ from this boot's reservations\n",
                board.name);
            return Err(ENODEV);
        }
        let mut digest = [0u8; 32];
        // SAFETY: the initialized private copy and distinct digest live for the
        // synchronous SHA-256 call.
        unsafe { bindings::sha256(canonical.as_ptr(), canonical.len(), digest.as_mut_ptr()) };
        if digest != image.text_sha256 {
            // Is it the 26.6.2 image, with only iBoot's GPU setup records
            // (outside the patchbay) differing from the J613's?
            let mut file = [0u8; 32];
            if normalize_setup_records(&mut canonical) {
                // SAFETY: as above.
                unsafe { bindings::sha256(canonical.as_ptr(), canonical.len(), file.as_mut_ptr()) };
            }
            if file != IMAGE_FILE_TEXT_SHA256 {
                dev_err!(pdev.as_ref(), "G16G: {}: unsupported normalized firmware SHA-256 {:02x?}\n",
                    board.name, digest);
                return Err(ENODEV);
            }
            if *crate::module_parameters::g16_j615_setup_records.value() != 1 {
                dev_err!(pdev.as_ref(),
                    "G16G: {}: the 26.6.2 GPU image, but iBoot's GPU setup records differ from the J613's ({:02x?}); not started. asahi.g16_j615_setup_records=1 accepts them; please report this line\n",
                    board.name, digest);
                return Err(ENODEV);
            }
            dev_warn!(pdev.as_ref(),
                "G16G: {}: the 26.6.2 GPU image with this Mac's own iBoot GPU setup records ({:02x?}), accepted by asahi.g16_j615_setup_records=1\n",
                board.name, digest);
            return Ok(Firmware { resources, board });
        }
    }
    let mut digest = [0u8; 32];
    // SAFETY: the initialized private copy and distinct digest live for the
    // synchronous SHA-256 call. Normalization never writes loaded firmware.
    unsafe { bindings::sha256(canonical.as_ptr(), canonical.len(), digest.as_mut_ptr()) };
    if digest != image.text_sha256 {
        dev_err!(
            pdev.as_ref(),
            "G16G: {}: unsupported normalized firmware SHA-256 {:02x?}\n",
            board.name, digest
        );
        return Err(ENODEV);
    }
    dev_info!(pdev.as_ref(), "G16G: {} firmware identified ({})\n", board.name, "RTKit-3255.160.4.release");
    Ok(Firmware { resources, board })
}

#[cfg(test)]
mod tests {
    use super::*;

    fn resources(delta: u64) -> g16_resources::Resources {
        let regions = [
            (0x103fffb8000, 0x4000),
            (0x103fff78000, 0x40000),
            (0x103fff70000, 0x4000),
            (0x103fff74000, 0x4000),
            (0x10000cc0000, TEXT_SIZE),
            (0x10001df4000, DATA_SIZE),
        ]
        .map(|(base, size)| g16_resources::Region {
            base: base + delta,
            size,
        });
        g16_resources::Resources::validate(
            1,
            regions,
            [0xffff_fc00_0000_0000, 0xffff_fc00_0006_0000],
            [1, 0],
        )
        .unwrap()
    }

    fn image_fixture() -> Vec<u8> {
        let mut bytes = vec![0xa5; TEXT_SIZE as usize];
        bytes[0x22c..0x234].copy_from_slice(&[0x4c, 0xce, 5, 0, 0x31, 2, 0, 0]);
        for (offset, key) in [(0x5ce4c, b"GKTS"), (0x5cf2f, b"ECAP")] {
            bytes[offset..offset + 4].copy_from_slice(key);
            bytes[offset + 4..offset + 8].copy_from_slice(&[8, 0, 0, 0]);
        }
        bytes
    }

    #[test]
    fn only_boot_entropy_is_normalized() {
        let mut a = image_fixture();
        let mut b = a.clone();
        b[J613_IMAGE.gkts+8..J613_IMAGE.gkts+16].fill(1);
        b[J613_IMAGE.ecap+8..J613_IMAGE.ecap+16].fill(2);
        assert!(normalize_boot_entropy(&mut a, &J613_IMAGE));
        assert!(normalize_boot_entropy(&mut b, &J613_IMAGE));
        assert!(a == b);
        b[0x4000] ^= 1;
        assert!(normalize_boot_entropy(&mut b, &J613_IMAGE));
        assert_ne!(a, b, "instruction changes must remain covered by the hash");
    }

    #[test]
    fn malformed_bootarg_headers_reject_without_mutation() {
        for offset in [0x22c, 0x230, J613_IMAGE.gkts, J613_IMAGE.gkts+4, J613_IMAGE.ecap, J613_IMAGE.ecap+4] {
            let mut bytes = image_fixture();
            bytes[offset] ^= 1;
            let original = bytes.clone();
            assert!(!normalize_boot_entropy(&mut bytes, &J613_IMAGE));
            assert!(bytes == original);
        }
        assert!(!normalize_boot_entropy(&mut [0; 64], &J613_IMAGE));
    }

    fn patchbay_fixture(regions: &[(u64, u64); 4]) -> Vec<u8> {
        let mut bytes = image_fixture();
        for &(i, v) in INFO_WORDS.iter() {
            bytes[INFO_HEADER + 4 * i..INFO_HEADER + 4 * i + 4].copy_from_slice(&v.to_le_bytes());
        }
        bytes[INFO_HEADER + 0x14..INFO_HEADER + 0x24].copy_from_slice(&IMAGE_UUID);
        // The 25G83 C0 patchbay's tag sequence and lengths, as loaded on a J613.
        let tags: [(&[u8; 4], usize); 31] = [(b"GKTS",8),(b"BPxG",8),(b"SPxG",8),(b"DILS",8),
            (b"SRSA",8),(b"GFCM",8),(b"SSSC",8),(b"qF8v",4),(b"LRSD",1),(b"SVSD",8),(b"LCSD",8),
            (b"ZSTR",4),(b"BPTP",8),(b"TNGI",1),(b"oeNS",1),(b"SEBC",8),(b"ECAP",8),(b"BVTP",8),
            (b"sP1T",8),(b"BtpG",8),(b"StpG",8),(b"BlpP",8),(b"S2xG",8),(b"SPTP",8),(b"SZSD",8),
            (b"LZSD",8),(b"LLTR",1),(b"SNUT",8),(b"ZNUT",4),(b"OTTR",8),(b"ARcM",4)];
        let mut at = PATCHBAY.start;
        for (tag, len) in tags {
            bytes[at..at + 4].copy_from_slice(tag);
            bytes[at + 4..at + 8].copy_from_slice(&(len as u32).to_le_bytes());
            let value: u64 = match tag {
                b"BtpG" => regions[0].0, b"BPTP" => regions[1].0, b"SPTP" => regions[1].1,
                b"BlpP" => regions[2].0, b"S2xG" => regions[3].0, b"BVTP" => 0xffff_fc00_0000_0000,
                _ => 0x5a5a_5a5a_5a5a_5a5a,
            };
            bytes[at + 8..at + 8 + len].copy_from_slice(&value.to_le_bytes()[..len]);
            at += 8 + len;
        }
        // Two placeholder records follow in the real image; pad to the end here.
        assert!(at <= PATCHBAY.end);
        if at < PATCHBAY.end {
            let pad = PATCHBAY.end - at - 8;
            bytes[at..at + 4].copy_from_slice(b"ZSOI");
            bytes[at + 4..at + 8].copy_from_slice(&(pad as u32).to_le_bytes());
            bytes[at + 8..PATCHBAY.end].fill(0x33);
        }
        bytes
    }

    const RUBRA: [(u64, u64); 4] = [(0x103fffb8000, 0x4000), (0x103fff78000, 0x40000),
        (0x103fff70000, 0x4000), (0x103fff74000, 0x4000)];

    #[test]
    fn patchbay_identity_ignores_carveout_placement_but_checks_it() {
        let moved = RUBRA.map(|(b, s)| (b - 0x200000000, s));
        let mut a = patchbay_fixture(&RUBRA);
        let mut b = patchbay_fixture(&moved);
        assert!(normalize_patchbay(&mut a, &RUBRA, 0xffff_fc00_0000_0000));
        assert!(normalize_patchbay(&mut b, &moved, 0xffff_fc00_0000_0000));
        assert!(a == b, "two RAM layouts give one identity");
        // Carveouts that are not the reserved regions refuse, unchanged.
        let mut c = patchbay_fixture(&RUBRA);
        let original = c.clone();
        assert!(!normalize_patchbay(&mut c, &moved, 0xffff_fc00_0000_0000));
        assert!(c == original);
        assert!(!normalize_patchbay(&mut c, &RUBRA, 0xffff_fc00_0004_0000));
        // Code outside the patchbay stays covered.
        let mut d = patchbay_fixture(&RUBRA);
        d[0x4000] ^= 1;
        assert!(normalize_patchbay(&mut d, &RUBRA, 0xffff_fc00_0000_0000));
        assert!(a != d);
    }

    #[test]
    fn setup_records_are_outside_the_patchbay_and_the_code() {
        for range in SETUP_WRITES.iter() {
            assert!(range.end <= PATCHBAY.start || range.start >= PATCHBAY.end);
            assert!(range.end <= TEXT_SIZE as usize);
        }
        // The header's identifying words (0-13) stay covered; 15 and 16 are iBoot's.
        assert!(SETUP_WRITES[0].start >= INFO_HEADER + 4 * 14);
        let mut a = patchbay_fixture(&RUBRA);
        assert!(normalize_patchbay(&mut a, &RUBRA, 0xffff_fc00_0000_0000));
        let mut b = a.clone();
        b[0x5d100] ^= 1;
        b[0x240] ^= 1;
        assert!(normalize_setup_records(&mut a) && normalize_setup_records(&mut b));
        assert!(a == b);
        b[0x4000] ^= 1;
        assert!(normalize_setup_records(&mut b));
        assert!(a != b);
        assert!(!normalize_setup_records(&mut [0; 64]));
    }

    #[test]
    fn patchbay_identity_rejects_other_headers_and_broken_lists() {
        for offset in [INFO_HEADER, INFO_HEADER + 4, INFO_HEADER + 8, INFO_HEADER + 0x14,
            INFO_HEADER + 0x2c, INFO_HEADER + 0x30, INFO_HEADER + 0x34] {
            let mut bytes = patchbay_fixture(&RUBRA);
            bytes[offset] ^= 1;
            assert!(!normalize_patchbay(&mut bytes, &RUBRA, 0xffff_fc00_0000_0000), "{offset:x}");
        }
        // A length running past the patchbay, and a missing carveout tag.
        let mut long = patchbay_fixture(&RUBRA);
        long[PATCHBAY.start + 4] = 0xff;
        assert!(!normalize_patchbay(&mut long, &RUBRA, 0xffff_fc00_0000_0000));
        let mut renamed = patchbay_fixture(&RUBRA);
        let at = renamed.windows(4).position(|w| w == b"BlpP").unwrap();
        renamed[at] = b'X';
        assert!(!normalize_patchbay(&mut renamed, &RUBRA, 0xffff_fc00_0000_0000));
        assert!(!normalize_patchbay(&mut [0; 64], &RUBRA, 0xffff_fc00_0000_0000));
    }

    #[test]
    fn firmware_identity_survives_physical_relocation() {
        for delta in [0, 0x400000000] {
            let fw = Firmware::from_digest(resources(delta), TEXT_SHA256).unwrap();
            assert_eq!(fw.version(), "RTKit-3255.160.4.release");
            assert_eq!(fw.resources.regions[4].base, 0x10000cc0000 + delta);
        }
    }

    #[test]
    fn unknown_code_and_different_layout_cannot_select_this_abi() {
        let mut changed = TEXT_SHA256;
        changed[0] ^= 1;
        assert!(Firmware::from_digest(resources(0), changed).is_none());
        let mut resized = resources(0);
        resized.regions[5].size += 0x4000;
        assert!(Firmware::from_digest(resized, TEXT_SHA256).is_none());
    }
}
