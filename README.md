# c-sike
SIKE/p434 in C

This is an implementation of SIKE/p434 used for experimentation.

# Performance results

## SIKEp434-sha256

```
| CPU                  | Operation     | P503 (ops/sec) | P434 (ops/sec) |
|----------------------|---------------|----------------|----------------|
| x86-64: with BMI2    | SIKE generate | 313.7          | 446.6          |
| x86-64: with BMI2    | SIKE encap    | 193.7          | 274.5          |
| x86-64: with BMI2    | SIKE decap    | 181.8          | 256.6          |
| ARM64                | SIKE generate | 136.4          | 182.1          |
| ARM64                | SIKE encap    | 80.5           | 108.4          |
| ARM64                | SIKE decap    | 76.1           | 102.0          |
| x86-64: without BMI2 | SIKE generate | 266.9          | 377.8          |
| x86-64: without BMI2 | SIKE encap    | 164.9          | 236.2          |
| x86-64: without BMI2 | SIKE decap    | 155.1          | 219.7          |
```

## BoringSSL patch

The ``CECPQ2b_for_BoringSSL_706da620b248615b709e71b36a31312f87a2c692.patch`` patch contains SIKE code that
can be applied directly on top of BoringSSL. Patch applies cleanly on commit ``706da620b248615b709e71b36a31312f87a2c692``.