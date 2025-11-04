# BranchCloak

> **BranchCloak: Mitigating Side-Channel Attacks on Directional Branch Predictors**  
> *Jihoon Kim, Hyerean Jang, and Youngjoo Shin*  
> *School of Cybersecurity, Korea University*  
> Published in **Electronics 2025, 14(9):1758**  
> [https://doi.org/10.3390/electronics14091758](https://doi.org/10.3390/electronics14091758)

---

## 🧩 Overview

**BranchCloak** is a novel **software-based mitigation technique** that protects processors from **Pattern History Table (PHT)** side-channel attacks on **directional branch predictors**.  
Instead of modifying hardware, BranchCloak obfuscates the PHT state by inserting *r-branches*—randomized dummy conditional branches—that collide with the target branch in the PHT.  
This randomization prevents attackers from inferring the branch direction of victim processes.

---

## ⚙️ Core Idea

PHT-based attacks (e.g., **BranchScope[1]**, **Bluethunder[2]**) exploit shared branch predictor state between cores.  
BranchCloak neutralizes this by ensuring:

- Each **r-branch**:
  1. Occupies the *same PHT entry* as the target (secret-dependent) branch.  
  2. Makes a *random branch decision* (50% taken / not-taken).

Even if an attacker probes the PHT, the randomized state reveals no information about the victim’s control flow.

---

## 📊 Evaluation

- Tested on **Intel Kaby Lake, Comet Lake, Rocket Lake** CPUs  
- Benchmarks: **OpenSSL 3.1.0**, **MbedTLS 3.1.0**, **Libgcrypt 1.9.4**  
- Targeted secret-dependent branches (e.g., RSA modular exponentiation, ECC scalar multiplication)  

---

<small><em>[1] Evtyushkin, Dmitry, et al. "Branchscope: A new side-channel attack on directional branch predictor." ACM SIGPLAN Notices 53.2 (2018): 693-707.</em></small>

<small><em>[2] Huo, Tianlin, et al. "Bluethunder: A 2-level directional predictor based side-channel attack against sgx." IACR Transactions on Cryptographic Hardware and Embedded Systems (2020): 321-347.</em></small>

