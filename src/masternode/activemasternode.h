// Copyright (c) 2014-2019 The Dash Core developers
// Copyright (c) 2020-2022 The Cosanta Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ACTIVEMASTERNODE_H
#define ACTIVEMASTERNODE_H

#include "chainparams.h"
#include "key.h"
#include "net.h"
#include "primitives/transaction.h"
#include "validationinterface.h"

#include "evo/deterministicmns.h"
#include "evo/providertx.h"

static const CAmount MASTERNODE_COLLATERAL_AMOUNT       = 10000 * COIN;

struct CActiveMasternodeInfo;
class CActiveMasternodeManager;

extern CActiveMasternodeInfo activeMasternodeInfo;
extern CActiveMasternodeManager* activeMasternodeManager;

struct CActiveMasternodeInfo {
    // Keys for the active Masternode
    std::unique_ptr<CBLSPublicKey> blsPubKeyOperator;
    std::unique_ptr<CBLSSecretKey> blsKeyOperator;

    // Initialized while registering Masternode
    uint256 proTxHash;
    COutPoint outpoint;
    CService service;
};


class CActiveMasternodeManager : public CValidationInterface
{
public:
    enum masternode_state_t {
        MASTERNODE_WAITING_FOR_PROTX,
        MASTERNODE_POSE_BANNED,
        MASTERNODE_REMOVED,
        MASTERNODE_OPERATOR_KEY_CHANGED,
        MASTERNODE_PROTX_IP_CHANGED,
        MASTERNODE_READY,
        MASTERNODE_ERROR,
    };

private:
    masternode_state_t state{MASTERNODE_WAITING_FOR_PROTX};
    std::string strError;

public:
    virtual void UpdatedBlockTip(const CBlockIndex* pindexNew, const CBlockIndex* pindexFork, bool fInitialDownload);

    void Init(const CBlockIndex* pindex);

    std::string GetStateString() const;
    std::string GetStatus() const;

    static bool IsValidNetAddr(CService addrIn);

private:
    bool GetLocalAddress(CService& addrRet);
};

#endif
