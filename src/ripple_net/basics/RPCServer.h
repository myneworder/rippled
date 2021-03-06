//------------------------------------------------------------------------------
/*
    Copyright (c) 2011-2013, OpenCoin, Inc.
*/
//==============================================================================

#ifndef RIPPLE_NET_BASICS_RPCSERVER_H_INCLUDED
#define RIPPLE_NET_BASICS_RPCSERVER_H_INCLUDED

/** Provides RPC services to a client.
    Each client has a separate instance of this object.
*/
class RPCServer
{
public:
    /** Handles a RPC client request.
    */
    class Handler
    {
    public:
        virtual ~Handler () { }

        /** Construct a HTTP response string.
        */
        virtual std::string createResponse (int statusCode, std::string const& description) = 0;

        /** Determine if the connection is authorized.
        */
        virtual bool isAuthorized (std::map <std::string, std::string> const& headers) = 0;

        /** Produce a response for a given request.

            @param  request The RPC request string.
            @return         The server's response.
        */
        virtual std::string processRequest (std::string const& request, std::string const& remoteAddress) = 0;
    };

    virtual ~RPCServer () { }

    /** Called when the connection is established.
    */
    virtual void connected () = 0;

    // VFALCO TODO Remove this since it exposes boost
    virtual boost::asio::ip::tcp::socket& getRawSocket () = 0;

    /** Retrieve the remote address as a string.
    
        @return A std::string representing the remote address.
    */
    // VFALCO TODO Replace the return type with a dedicated class.
    virtual std::string getRemoteAddressText () = 0;
};

#endif
