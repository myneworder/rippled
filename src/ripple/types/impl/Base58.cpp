//------------------------------------------------------------------------------
/*
    Copyright (c) 2011-2013, OpenCoin, Inc.
*/
//==============================================================================

// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2011 The Bitcoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

namespace ripple {

char const* Base58::s_currentAlphabet = Base58::getRippleAlphabet ();

void Base58::fourbyte_hash256 (void* out, void const* in, std::size_t bytes)
{
    unsigned char const* const p (
        static_cast <unsigned char const*>(in));
    uint256 hash (SHA256Hash (p, p + bytes));
    memcpy (out, hash.begin(), 4);
}

char const* Base58::getBitcoinAlphabet ()
{
    return "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
}

char const* Base58::getRippleAlphabet ()
{
    return "rpshnaf39wBUDNEGHJKLM4PQRST7VWXYZ2bcdeCg65jkm8oFqi1tuvAxyz";
}

char const* Base58::getTestnetAlphabet ()
{
    return "RPShNAF39wBUDnEGHJKLM4pQrsT7VWXYZ2bcdeCg65jkm8ofqi1tuvaxyz";
}

std::string Base58::raw_encode (
    unsigned char const* begin, unsigned char const* end,
        char const* alphabet, bool withCheck)
{
    CAutoBN_CTX pctx;
    CBigNum bn58 = 58;
    CBigNum bn0 = 0;

    // Convert little endian data to bignum
    CBigNum bn (begin, end);
    std::size_t const size (std::distance (begin, end));

    // Convert bignum to std::string
    std::string str;
    // Expected size increase from base58 conversion is approximately 137%
    // use 138% to be safe
    str.reserve (size * 138 / 100 + 1);
    CBigNum dv;
    CBigNum rem;

    while (bn > bn0)
    {
        if (!BN_div (&dv, &rem, &bn, &bn58, pctx))
            throw bignum_error ("EncodeBase58 : BN_div failed");

        bn = dv;
        unsigned int c = rem.getuint ();
        str += alphabet [c];
    }
    
    for (const unsigned char* p = end-2; p >= begin && *p == 0; p--)
        str += alphabet [0];

    // Convert little endian std::string to big endian
    reverse (str.begin (), str.end ());
    return str;
}

char const* Base58::getCurrentAlphabet ()
{
    return s_currentAlphabet;
}

void Base58::setCurrentAlphabet (char const* alphabet)
{
    s_currentAlphabet = alphabet;
}

//------------------------------------------------------------------------------

bool Base58::decode (const char* psz, Blob& vchRet, const char* pAlpha)
{
    assert (pAlpha != 0);

    CAutoBN_CTX pctx;
    vchRet.clear ();
    CBigNum bn58 = 58;
    CBigNum bn = 0;
    CBigNum bnChar;

    while (isspace (*psz))
        psz++;

    // Convert big endian string to bignum
    for (const char* p = psz; *p; p++)
    {
        const char* p1 = strchr (pAlpha, *p);

        if (p1 == NULL)
        {
            while (isspace (*p))
                p++;

            if (*p != '\0')
                return false;

            break;
        }

        bnChar.setuint (p1 - pAlpha);

        if (!BN_mul (&bn, &bn, &bn58, pctx))
            throw bignum_error ("DecodeBase58 : BN_mul failed");

        bn += bnChar;
    }

    // Get bignum as little endian data
    Blob vchTmp = bn.getvch ();

    // Trim off sign byte if present
    if (vchTmp.size () >= 2 && vchTmp.end ()[-1] == 0 && vchTmp.end ()[-2] >= 0x80)
        vchTmp.erase (vchTmp.end () - 1);

    // Restore leading zeros
    int nLeadingZeros = 0;

    for (const char* p = psz; *p == pAlpha[0]; p++)
        nLeadingZeros++;

    vchRet.assign (nLeadingZeros + vchTmp.size (), 0);

    // Convert little endian data to big endian
    std::reverse_copy (vchTmp.begin (), vchTmp.end (), vchRet.end () - vchTmp.size ());
    return true;
}

bool Base58::decode (const std::string& str, Blob& vchRet)
{
    return decode (str.c_str (), vchRet);
}

bool Base58::decodeWithCheck (const char* psz, Blob& vchRet, const char* pAlphabet)
{
    assert (pAlphabet != NULL);

    if (!decode (psz, vchRet, pAlphabet))
        return false;

    if (vchRet.size () < 4)
    {
        vchRet.clear ();
        return false;
    }

    uint256 hash = SHA256Hash (vchRet.begin (), vchRet.end () - 4);

    if (memcmp (&hash, &vchRet.end ()[-4], 4) != 0)
    {
        vchRet.clear ();
        return false;
    }

    vchRet.resize (vchRet.size () - 4);
    return true;
}

bool Base58::decodeWithCheck (const std::string& str, Blob& vchRet, const char* pAlphabet)
{
    return decodeWithCheck (str.c_str (), vchRet, pAlphabet);
}

}

