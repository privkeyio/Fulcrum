# Fulcrum — Oracle Knots compatibility

This branch adds support for running Fulcrum against an **[Oracle Knots](https://github.com/privkeyio)** full node
(the BLAKE2b "sovereign" fork of Bitcoin Knots).

## Lineage / attribution

- Upstream: **Fulcrum** by Calin A. Culianu — <https://github.com/cculianu/Fulcrum> (GPLv3).
- BLAKE2b support: **privkeyio/Fulcrum** — <https://github.com/privkeyio/Fulcrum> (parses 164‑byte v2 BLAKE2b headers).
- This branch: **MarcanoFilms/Fulcrum** — adds Oracle Knots user-agent detection on top of the above.

Licensed under GPLv3, same as upstream. Only the change described below was added.

## The problem

Fulcrum decides whether the backing node is **BTC (SegWit)** or **BCH (non‑SegWit)** from the daemon's
`getnetworkinfo` **subversion** string. Stock Bitcoin Core / Bitcoin Knots report `"/Satoshi:x.y.z/..."`,
which Fulcrum recognizes as BTC.

**Oracle Knots** reports its own user agent, `"/OracleKnots:x.y.z/..."`. Fulcrum did not recognize this
prefix, fell back to its default (**BCH, non‑SegWit**), and then aborted at the first SegWit block:

```
FATAL: SegWit block encountered for non-SegWit coin.
```

## The change

One line in [`src/BitcoinD.cpp`](src/BitcoinD.cpp) — also treat the `/OracleKnots:` user agent as a
Core/BTC (SegWit-enabled) node:

```cpp
isCore = subversion.startsWith("/Satoshi:")            // Bitcoin Core / Bitcoin Knots
         || subversion.startsWith("/OracleKnots:");    // Oracle Knots (BLAKE2b sovereign fork)
```

With this, Fulcrum logs `Coin: BTC`, crosses SegWit activation normally, and indexes the whole chain.

## Build & run

Build is unchanged from upstream (`qmake && make`). Point it at your Oracle Knots node's RPC in
`fulcrum.conf`:

```
bitcoind = 127.0.0.1:8332
rpcuser  = <your-rpcuser>
rpcpassword = <your-rpcpassword>
datadir  = /path/on/fast/ssd
tcp = 0.0.0.0:50001
ssl = 0.0.0.0:50002
cert = /path/fulcrum.crt
key  = /path/fulcrum.key
peering = false
announce = false
```

Prebuilt Linux x86_64 binaries (with SHA256 checksums) are attached to the GitHub Release.
