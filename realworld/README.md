# Testing BranchCloak on Realworld Application

## Overview

This test-suite runs BranchCloak’s evaluation on three popular cryptographic libraries used in the paper:

- **Libgcrypt 1.9.4**
- **MbedTLS 3.1.0**
- **OpenSSL 3.1.0**

Each library directory contains example `.c` files that include the vulnerable (secret-dependent) functions instrumented for BranchCloak testing. A convenience script `run.py` is provided under each library’s subfolder to execute the test workflow.

---

## Repository layout

```
/realworld/
  ├─ libgcrypt/
  │   ├─ align/
  │   ├─ raw/
  │   ├─ run.py
  │   ├─ run.sh
  │   ├─ t-ed25519
  │   └─ t-ed25519.inp
  ├─ mbedtls/
  │   ├─ align/
  │   ├─ raw/
  │   ├─ run.py
  │   ├─ run.sh
  │   └─ test_suite_mpi.datax
  ├─ openssl/
  │   ├─ align/
  │   ├─ raw/
  │   ├─ run.py
  │   ├─ run.sh
  │   └─ bntest
  ├─ libgcrypt_ec.c
  ├─ mbedtls_bignum.c
  └─ openssl_bn_exp.c
```

- Each library folder (`libgcrypt/`, `mbedtls/`, `openssl/`) contains two build variants:
  - `align/` — binaries (or `.so` files) built **with BranchCloak applied** (i.e., r-branches inserted and aligned).
  - `raw/` — original binaries (or `.so` files) **without BranchCloak** (baseline).
- `run.sh` — shell wrapper that runs the library's test program (e.g., `t-ed25519`, `test_suite_mpi`, `bntest`) for both the raw and BranchCloak-enabled builds, collecting results for comparison.
- `run.py` — Python harness that invokes `run.sh`, orchestrates experiments, handles `perf` collection, and aggregates / prints the measurement summary.
- Test artifacts:
  - `t-ed25519`, `t-ed25519.inp` (Libgcrypt)
  - `test_suite_mpi.datax` (MbedTLS)
  - `bntest` (OpenSSL)
  These are the test inputs / drivers used by the `run.sh` scripts.
- Top-level C files (`libgcrypt_ec.c`, `mbedtls_bignum.c`, `openssl_bn_exp.c`) are **example source files** that contain the vulnerable, secret-dependent branches used in the experiments.

---

## Prerequisites

1. **Python 3** (to run `run.py`)
2. **linux-perf** (perf): used to collect hardware performance counters.
3. A Linux system with `perf` enabled and permissions to collect counters (require root/sudo).

> **Note:** Performance counter *names* vary across CPU architectures. If perf events referenced in `run.py` fail, check `perf list` and adapt the counter names in the script accordingly.

---

## How to run the tests

1. Open a terminal and `cd` into the library subfolder you want to test, for example:

```
cd ./libgcrypt
```

2. Run the test script with Python 3. The script requires sudo access to use `perf`. The repository’s test convention expects the sudo password to be passed as an argument:

```
python3 run.py <sudo password>
```

(Replace `<sudo password>` with your actual sudo password according to your environment / local policy.)

3. The `run.py` does the following (automated):
   - Compiles the example C file with BranchCloak-enabled build flags (if applicable).
   - Executes the example workload multiple iterations.
   - Collects performance counters via `perf` (execution time, branch-mispredictions, cache metrics, etc.).
