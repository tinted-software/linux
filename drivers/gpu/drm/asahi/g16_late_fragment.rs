// SPDX-License-Identifier: GPL-2.0-only OR MIT

//! Eligibility for a bounded re-poll, never an alternative retirement test.

#[derive(Clone, Copy, Default)]
pub(super) struct Witnesses {
    pub expected: u32,
    pub firmware: u32,
    pub user: u32,
    pub ta: [u32; 2],
    pub timestamps: [[u64; 2]; 2],
    pub queue_done: bool,
    pub pipe_done: bool,
    pub private_done: bool,
}

fn execution_complete(w: Witnesses) -> bool {
    w.expected != 0
        && w.user == w.expected
        && w.ta == [w.expected; 2]
        && w.timestamps.iter().all(|[start, end]| *start != 0 && end > start)
        && w.queue_done && w.pipe_done && w.private_done
}

pub(super) fn repoll(w: Witnesses, elapsed_ns: i64) -> bool {
    // Equality deliberately excludes future/shared stamps and other lag sizes.
    // Wrapping subtraction follows the existing u32 notifier stamp domain.
    (2_000_000_000..4_000_000_000).contains(&elapsed_ns)
        && w.firmware == w.expected.wrapping_sub(0x100)
        && execution_complete(w)
}

pub(super) fn reconciled(w: Witnesses) -> bool {
    // A grace-path retirement must freshly revalidate every witness, including
    // the now-complete firmware stamp. No previous snapshot can substitute.
    w.firmware == w.expected && execution_complete(w)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn dump() -> Witnesses {
        Witnesses { expected: 0x671ff00, firmware: 0x671fe00,
            user: 0x671ff00, ta: [0x671ff00; 2],
            timestamps: [[0x40b96d9757, 0x40b96d9942],
                         [0x40b96d9c37, 0x40b96da1dc]],
            queue_done: true, pipe_done: true, private_done: true }
    }

    #[test]
    fn bounded_dump_signature() {
        assert!(!repoll(dump(), 1_999_999_999));
        assert!(repoll(dump(), 2_000_000_000));
        assert!(repoll(dump(), 3_999_999_999));
        assert!(!repoll(dump(), 4_000_000_000));
        assert!(!repoll(dump(), i64::MAX));
    }

    #[test]
    fn stamp_arrival_requires_fresh_witnesses() {
        let mut w = dump();
        assert!(!reconciled(w));
        w.firmware = w.expected;
        assert!(reconciled(w));
        w.firmware += 0x100;
        assert!(!reconciled(w));
        w.firmware = w.expected;
        for stage in 0..2 {
            let mut bad = w; bad.ta[stage] -= 0x100;
            assert!(!reconciled(bad));
            let mut bad = w; bad.timestamps[stage][1] = 0;
            assert!(!reconciled(bad));
        }
        for field in 0..4 {
            let mut bad = w;
            match field { 0 => bad.queue_done = false, 1 => bad.pipe_done = false,
                2 => bad.private_done = false, _ => bad.user -= 0x100 }
            assert!(!reconciled(bad));
        }
    }

    #[test]
    fn every_witness_is_required() {
        let d = dump();
        let mut bad = d; bad.expected = 0; assert!(!repoll(bad, 2_000_000_000));
        for value in [d.expected, d.expected - 0x200, d.expected + 0x100] {
            let mut bad = d; bad.firmware = value; assert!(!repoll(bad, 2_000_000_000));
        }
        for value in [0, d.expected - 0x100, d.expected + 0x100] {
            let mut bad = d; bad.user = value; assert!(!repoll(bad, 2_000_000_000));
        }
        for stage in 0..2 {
            let mut bad = d; bad.ta[stage] = d.expected - 0x100;
            assert!(!repoll(bad, 2_000_000_000));
            for pair in [[0, 1], [1, 0], [1, 1], [2, 1]] {
                let mut bad = d; bad.timestamps[stage] = pair;
                assert!(!repoll(bad, 2_000_000_000));
            }
        }
        for field in 0..3 {
            let mut bad = d;
            match field { 0 => bad.queue_done = false, 1 => bad.pipe_done = false,
                _ => bad.private_done = false }
            assert!(!repoll(bad, 2_000_000_000));
        }
    }
}
