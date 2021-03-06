//------------------------------------------------------------------------------
/*
    Copyright (c) 2011-2013, OpenCoin, Inc.
*/
//==============================================================================

#ifndef RIPPLE_DATABASECON_H
#define RIPPLE_DATABASECON_H

// VFALCO NOTE This looks like a pointless class. Figure out
//         what purpose it is really trying to serve and do it better.
class DatabaseCon : LeakChecked <DatabaseCon>
{
public:
    DatabaseCon (const std::string& name, const char* initString[], int countInit);
    ~DatabaseCon ();
    Database* getDB ()
    {
        return mDatabase;
    }
    DeprecatedRecursiveMutex& getDBLock ()
    {
        return mLock;
    }
    static int getCount ()
    {
        return sCount;
    }

    // VFALCO TODO change "protected" to "private" throughout the code
private:
    Database*               mDatabase;
    DeprecatedRecursiveMutex  mLock;
    static int              sCount;
};

#endif

