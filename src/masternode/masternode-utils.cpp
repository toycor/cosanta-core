// Copyright (c) 2014-2020 The Dash Core developers
// Copyright (c) 2020-2021 The Cosanta Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <masternode/masternode-utils.h>

#include <init.h>
#include <masternode/masternode-sync.h>
#ifdef ENABLE_WALLET
#include <privatesend/privatesend-client.h>
#endif
#include <validation.h>

struct CompareScoreMN
{
    bool operator()(const std::pair<arith_uint256, const CDeterministicMNCPtr&>& t1,
                    const std::pair<arith_uint256, const CDeterministicMNCPtr&>& t2) const
    {
        return (t1.first != t2.first) ? (t1.first < t2.first) : (t1.second->collateralOutpoint < t2.second->collateralOutpoint);
    }
};

void CMasternodeUtils::ProcessMasternodeConnections(CConnman& connman)
{
    std::vector<CDeterministicMNCPtr> vecDmns; // will be empty when no wallet
#ifdef ENABLE_WALLET
    privateSendClient.GetMixingMasternodesInfo(vecDmns);
#endif // ENABLE_WALLET

    // Don't disconnect masternode connections when we have less then the desired amount of outbound nodes
    int nonMasternodeCount = 0;
    connman.ForEachNode(CConnman::AllNodes, [&](CNode* pnode) {
        if (!pnode->fInbound && !pnode->fFeeler && !pnode->m_manual_connection && !pnode->fMasternode && !pnode->fMasternodeProbe) {
            nonMasternodeCount++;
        }
    });
    if (nonMasternodeCount < connman.GetMaxOutboundNodeCount()) {
        return;
    }

    connman.ForEachNode(CConnman::AllNodes, [&](CNode* pnode) {
        if (pnode->fMasternode && !connman.IsMasternodeQuorumNode(pnode)) {
#ifdef ENABLE_WALLET
            bool fFound = false;
            for (const auto& dmn : vecDmns) {
                if (pnode->addr == dmn->pdmnState->addr) {
                    fFound = true;
                    break;
                }
            }
            if (fFound) return; // do NOT disconnect mixing masternodes
#endif // ENABLE_WALLET
            if (fLogIPs) {
                LogPrintf("Closing Masternode connection: peer=%d, addr=%s\n", pnode->GetId(), pnode->addr.ToString());
            } else {
                LogPrintf("Closing Masternode connection: peer=%d\n", pnode->GetId());
            }
            pnode->fDisconnect = true;
        }
    });
}

void CMasternodeUtils::DoMaintenance(CConnman& connman)
{
    if(!masternodeSync.IsBlockchainSynced() || ShutdownRequested())
        return;

    static unsigned int nTick = 0;

    nTick++;

    if(nTick % 60 == 0) {
        ProcessMasternodeConnections(connman);
    }
}

