// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <script/standard.h>
#include <script/sign.h>
#include <streams.h>
#include <tinyformat.h>
#include <utilstrencodings.h>
#include <crypto/common.h>
#include <keystore.h>
#include <pos_kernel.h>

uint256 CBlockHeader::GetHash() const
{
    if (IsProofOfStake()) {
        CDataStream sh(SER_GETHASH, 0);

        sh << *this;
        /*
        egihash::h256_t blockHash(sh.data(), sh.size());
        return uint256(blockHash);
        */
    }

    std::vector<unsigned char> vch(80);
    CVectorWriter ss(SER_NETWORK, PROTOCOL_VERSION, vch, 0);
    ss << *this;
    return HashX11((const char *)vch.data(), (const char *)vch.data() + vch.size());
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    if (IsProofOfStake()) {
        /*
         s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nHeight=%u, hashMix=%s, nNonce=%u, posStakeHash=%s, posStakeN=%u, posPubKey=%u, posBlockSig=%u vtx=%u)\n",
                        GetHash().ToString(),
                        nVersion,
                        hashPrevBlock.ToString(),
                        hashMerkleRoot.ToString(),
                        nTime, nBits, nHeight,
                        hashMix.ToString(),
                        nNonce,
                        posStakeHash.ToString(),
                        posStakeN,
                        posPubKey.size(),
                        posBlockSig.size(),
                        vtx.size());
        */
    }else{
        s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
            GetHash().ToString(),
            nVersion,
            hashPrevBlock.ToString(),
            hashMerkleRoot.ToString(),
            nTime, nBits, nNonce,
            vtx.size());
    }

    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}


// ppcoin: sign block
bool CBlockHeader::SignBlock(const CKeyStore& keystore)
{
    if (IsProofOfStake())
    {
        if (!posPubKey.IsValid())
            return false;

        CKey key;

        if (!keystore.GetKey(posPubKey.GetID(), key)) {
            return false;
        }

        if (!key.SignCompact(GetHash(), posBlockSig)) {
            return false;
        }

        return true;
    }

    return true;
}

bool CBlockHeader::CheckBlockSignature(const CKeyID& key_id) const
{
    if (!IsProofOfStake()) {
        return true;
    }

    if (posBlockSig.empty()) {
        return false;
    }

    auto hash = GetHash();
    posPubKey.RecoverCompact(hash, posBlockSig);

    if (!posPubKey.IsValid()) {
        return false;
    }

    return posPubKey.GetID() == key_id;
}

const CPubKey& CBlockHeader::BlockPubKey() const
{
    // In case it's read from disk
    if (!posPubKey.IsValid() && !posBlockSig.empty()) {
        posPubKey.RecoverCompact(GetHash(), posBlockSig);
    }

    return posPubKey;
}

bool CBlock::HasCoinBase() const {
    return (!vtx.empty() && CoinBase()->IsCoinBase());
}

bool CBlock::HasStake() const {
    if (!IsProofOfStake() || (vtx.size() < 2)) {
        return false;
    }

    BlockPubKey();

    if (!posPubKey.IsValid()) {
        return false;
    }

    const auto spk = GetScriptForDestination(posPubKey.GetID());
    const auto& cb_vout = CoinBase()->vout;
    const auto& stake = Stake();

    if (cb_vout.empty() || stake->vin.empty() || stake->vout.empty()) {
        return false;
    }

    // Check it's the same stake
    if (stake->vin[0].prevout != COutPoint(posStakeHash, posStakeN)) {
        return false;
    }

    // Check primary coinbase output
    if (cb_vout[0].scriptPubKey != spk) {
        return false;
    }

    // Check stake outputs
    CAmount total_amt = 0;

    for (const auto &so : stake->vout) {
        if (so.scriptPubKey != spk) {
            return false;
        }

        total_amt += so.nValue;
    }

    if (total_amt < MIN_STAKE_AMOUNT) {
        return false;
    }

    return true;
}
