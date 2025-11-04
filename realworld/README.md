# Testing BranchCloak on realworld application

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
  │   ├─ run.py
  │   └─ ...
  ├─ mbedtls/
  │   ├─ run.py
  │   └─ ...
  ├─ openssl/
  │   ├─ run.py
  │   └─ ...
  ├─ libgcrypt_ec.c
  ├─ mbedtls_bignum.c
  └─ openssl_bn_exp.c
```

- Each `.c` contains a secret-dependent branch in realworld applications targeted by BranchCloak.
- Each `run.py` automates compile/run/perf collection for that library’s example.

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
