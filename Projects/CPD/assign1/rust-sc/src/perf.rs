use std::{error::Error, time::Instant};

use perf_event::{Builder, Group, events::*};

const ACCESS_L1D_READ: Cache = Cache {
    which: CacheId::L1D,
    operation: CacheOp::READ,
    result: CacheResult::ACCESS,
};

const MISS_L1D_READ: Cache = Cache {
    result: CacheResult::MISS,
    ..ACCESS_L1D_READ
};

// const ACCESS_L1D_WRITE: Cache = Cache {
//     which: CacheId::L1D,
//     operation: CacheOp::WRITE,
//     result: CacheResult::ACCESS,
// };

// const MISS_L1D_WRITE: Cache = Cache {
//     result: CacheResult::MISS,
//     ..ACCESS_L1D_READ
// };

pub fn perf_bench<F>(f: F) -> Result<(), Box<dyn Error>>
where
    F: FnOnce() -> (),
{
    let mut group = Group::new()?;

    let access_l1d_read = group.add(&Builder::new(ACCESS_L1D_READ)).map_err(|e| {
        eprintln!("{e}");
        e
    });

    let miss_l1d_read = group.add(&Builder::new(MISS_L1D_READ)).map_err(|e| {
        eprintln!("{e}");
        e
    });

    let ll_access = group.add(&Builder::new(
        perf_event::events::Hardware::CACHE_REFERENCES,
    ))?;

    let ll_miss = group.add(&Builder::new(perf_event::events::Hardware::CACHE_MISSES))?;

    let now = Instant::now();
    group.enable()?;
    f();
    group.disable()?;
    let took = now.elapsed();
    println!("Took {}s", took.as_secs_f64());

    let counts = group.read()?;

    let l1d_access = access_l1d_read.map(|s| counts[&s]).unwrap_or(0);
    let l1d_miss = miss_l1d_read.map(|s| counts[&s]).unwrap_or(0);

    let ll_access = counts[&ll_access];
    let ll_miss = counts[&ll_miss];

    println!("L1D_ACCESS: {l1d_access}",);
    println!("L1D_MISS: {l1d_miss}",);
    println!(
        "L1D RATIO: {:.002}%",
        (1.0 - (l1d_miss as f64 / l1d_access as f64)) * 100.
    );

    println!("LL_ACCESS: {ll_access}",);
    println!("LL_MISS: {ll_miss}",);
    println!(
        "LL RATIO: {:.002}%",
        (1.0 - (ll_miss as f64 / ll_access as f64)) * 100.
    );

    Ok(())
}
