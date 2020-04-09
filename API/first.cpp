//g++ first.cpp -o first.o -lPocoFoundation -lPocoNet -lPocoUtil -lPocoJSON

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/JSON.h"
#include "Poco/Dynamic/Var.h"
#include <iostream>
#include <string>

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco::JSON;
using namespace std;

class HelloRequestHandler: public HTTPRequestHandler
{
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {


        if(request.getURI() == "/"){
		    Parser parser;
		    int i = 0;
            std::string str = "[{\"name\":\"Murilo\",\"age\":29},{\"name\":\"Murilo 2\",\"age\":29},{\"name\":\"Murilo 3\",\"age\":29},{\"name\":\"Murilo 4\",\"age\":29}]";
		    Poco::Dynamic::Var result = parser.parse(str);
		    Array::Ptr object = result.extract<Array::Ptr>();
		    Application& app = Application::instance();
		    while(object->getObject(i)){
			    app.logger().information(object->getObject(i)->getValue<string>("name"));
			    app.logger().information(object->getObject(i)->getValue<string>("age"));
			    i++;
		    }
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");

            std::ostream& ostr = response.send();

            ostr << str; 
        }

        else if(request.getURI() == "/order" && request.getMethod() == "POST"){
            std::istream& istr = request.stream();
            Application& app = Application::instance();
            int len = request.getContentLength();
            char* buffer = new char[len];
            istr.read(buffer, len);
            Parser parser;
            Poco::Dynamic::Var result = parser.parse(buffer);
            Array::Ptr arr = result.extract<Array::Ptr>();
            Object::Ptr object = arr->getObject(0);
            app.logger().information(object->get("name").convert<std::string>());
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");

            std::ostream& ostr = response.send();
        }
    }
};

class HelloRequestHandlerFactory: public HTTPRequestHandlerFactory
{
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest&)
    {
        return new HelloRequestHandler;
    }
};

class WebServerApp: public ServerApplication
{
    void initialize(Application& self)
    {
        loadConfiguration();
        ServerApplication::initialize(self);
    }

    int main(const std::vector<std::string>&)
    {
        UInt16 port = static_cast<UInt16>(config().getUInt("port", 8081));

        HTTPServer srv(new HelloRequestHandlerFactory, port);
        srv.start();
        logger().information("HTTP Server started on port %hu.", port);
        waitForTerminationRequest();
        logger().information("Stopping HTTP Server...");
        srv.stop();

        return Application::EXIT_OK;
    }
};

POCO_SERVER_MAIN(WebServerApp)
