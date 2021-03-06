//------------------------------------------------------------------------------
/*
    Copyright (c) 2011-2013, OpenCoin, Inc.
*/
//==============================================================================

#ifndef RIPPLE_PROTOCOL_H
#define RIPPLE_PROTOCOL_H

/** Protocol specific constants, types, and data.

    This information is part of the Ripple protocol. Specifically,
    it is required for peers to be able to communicate with each other.

    @note Changing these will create a hard fork.

    @ingroup protocol
    @defgroup protocol
*/
struct Protocol
{
    /** Smallest legal byte size of a transaction.
    */
    static int const txMinSizeBytes = 32;

    /** Largest legal byte size of a transaction.
    */
    static int const txMaxSizeBytes = 1024 * 1024; // 1048576
};

/** A ledger index.
*/
// VFALCO TODO pick one. I like Index since its not an abbreviation
typedef uint32 LedgerIndex;
// VFALCO NOTE "LedgerSeq" appears in some SQL statement text
typedef uint32 LedgerSeq;

/** A transaction identifier.
*/
// VFALCO TODO maybe rename to TxHash
typedef uint256 TxID;

/** A transaction index.
*/
typedef uint32 TxSeq; // VFALCO NOTE Should read TxIndex or TxNum

/** An account hash.

    The hash is used to uniquely identify the account.
*/
//typedef uint160 AccountHash;
//typedef uint260 ValidatorID;

#endif
