// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2011-2012 Litecoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of // Yo dawg, this is the secret. Checkpoint 0 hash == Genesis block hash.
                  (0, uint256("0x037a4f9134d0fba6774e7e3f89482c6fa2b96af8c358cb00a195f6111d78bd37"))
                  (184, uint256("0x8d16c895e877f13b28200d1d282263177bd029e88132242d3e0edeb96433bd65"))
                  (644, uint256("0x88cdad82a62f887d552f70e5b60dbf61c74d83474858cb0324d29265a52b09c8"))
                  (720, uint256("0x9c357633fa61f1cdee3fd100f529167a55b2637b43f5852ade82ef65fb754690"))
                  (7848, uint256("0xc1c7e20e17329f11746a781a43de032b80c4f64a974bd60bf666c328a38410cf"))
                  (12917, uint256("0x191244078565b8a965ee26332dae5a285c38bcd06f34e79be360724d9457c00b"))
                  (15339, uint256("0x460580e41f1aa52250af4814af3aa401f1df00fc34405fa24ad3125fcd608e1b"))
                  (24725, uint256("0xa1737a08f9dcd14531ba6f74cb2de552aa2525c19ceac05d7d3b39f89d1d71ba"))
            ;

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (fTestNet) return true; // Testnet has no checkpoints

        MapCheckpoints::const_iterator i = mapCheckpoints.find(nHeight);
        if (i == mapCheckpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        if (fTestNet) return 0;
        return mapCheckpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (fTestNet) return NULL;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, mapCheckpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
