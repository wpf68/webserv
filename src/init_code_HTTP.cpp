/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_code_HTTP.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 11:06:49 by pwolff            #+#    #+#             */
/*   Updated: 2022/12/05 11:06:49 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

extern std::map<std::string, std::string>   var_content_code;

// source : https://fr.wikipedia.org/wiki/Liste_des_codes_HTTP  //

void    ft_init_code_type(void)
{
    var_content_code["100"] = "Continue";
    var_content_code["101"] = "Switching Protocols";
    var_content_code["102"] = "Continue";
    var_content_code["103"] = "Continue";

    var_content_code["200"] = "OK";
    var_content_code["201"] = "Created";
    var_content_code["202"] = "Accepted";
    var_content_code["203"] = "Non-Authoritative Information";
    var_content_code["204"] = "No Content";
    var_content_code["205"] = "Reset Content";
    var_content_code["206"] = "Partial Content";
    var_content_code["207"] = "Multi-Status";
    var_content_code["208"] = "Already Reported";
    var_content_code["210"] = "Content Different";
    var_content_code["228"] = "IM Used";

    var_content_code["300"] = "Multiple Choices";
    var_content_code["301"] = "Moved Permanently";
    var_content_code["302"] = "Found";
    var_content_code["303"] = "See Other";
    var_content_code["304"] = "Not Modified";
    var_content_code["305"] = "Use Proxy (depuis HTTP/1.1) ";
    var_content_code["306"] = "Switch Proxy";
    var_content_code["307"] = "Temporary Redirect";
    var_content_code["308"] = "Permanent Redirect";
    var_content_code["310"] = "Too many Redirects";

    var_content_code["400"] = "Bad Request";
    var_content_code["401"] = "Unauthorized";
    var_content_code["402"] = "Payment Required";
    var_content_code["403"] = "Forbidden";
    var_content_code["404"] = "Not Found";
    var_content_code["405"] = "Method Not Allowed";
    var_content_code["406"] = "Not Acceptable";
    var_content_code["408"] = "Request Time-out";
    var_content_code["409"] = "Conflict";
    var_content_code["407"] = "Proxy Authentication Required";
    var_content_code["410"] = "Gone";
    var_content_code["411"] = "Length Required";
    var_content_code["412"] = "Precondition Failed";
    var_content_code["413"] = "Request Entity Too Large";
    var_content_code["414"] = "Request-URI Too Long";
    var_content_code["415"] = "Unsupported Media Type";
    var_content_code["416"] = "Requested range unsatisfiable";
    var_content_code["417"] = "Expectation failed";
    var_content_code["418"] = "I’m a teapot";
    var_content_code["421"] = "Bad mapping / Misdirected Request";
    var_content_code["422"] = "Unprocessable entity";
    var_content_code["423"] = "Locked";
    var_content_code["424"] = "Method failure";
    var_content_code["425"] = "Too Early";
    var_content_code["426"] = "Upgrade Required";
    var_content_code["428"] = "Precondition Required";
    var_content_code["429"] = "Too Many Requests";
    var_content_code["431"] = "Request Header Fields Too Large";
    var_content_code["449"] = "Retry With";
    var_content_code["450"] = "Blocked by Windows Parental Controls";
    var_content_code["451"] = "Unavailable For Legal Reasons";
    var_content_code["456"] = "Unrecoverable Error";

    var_content_code["444"] = "No Response";
    var_content_code["495"] = "SSL Certificate Error";
    var_content_code["496"] = "SSL Certificate Required";
    var_content_code["497"] = "HTTP Request Sent to HTTPS Port";
    var_content_code["498"] = "Token expired/invalid";
    var_content_code["499"] = "Client Closed Request";

    var_content_code["500"] = "Internal Server Error";
    var_content_code["501"] = "Not Implemented";
    var_content_code["502"] = "Bad Gateway ou Proxy Error";
    var_content_code["503"] = "Service Unavailable";
    var_content_code["504"] = "Gateway Time-out";
    var_content_code["505"] = "HTTP Version not supported";
    var_content_code["506"] = "Variant Also Negotiates";
    var_content_code["507"] = "Insufficient storage";
    var_content_code["508"] = "Loop detected";
    var_content_code["509"] = "Bandwidth Limit Exceeded";
    var_content_code["510"] = "Not extended";
    var_content_code["511"] = "Network authentication required";

    var_content_code["520"] = "Unknown Error";
    var_content_code["521"] = "Web Server Is Down";
    var_content_code["522"] = "Connection Timed Out";
    var_content_code["523"] = "Origin Is Unreachable";
    var_content_code["524"] = "A Timeout Occurred";
    var_content_code["525"] = "SSL Handshake Failed";
    var_content_code["526"] = "Invalid SSL Certificate";
    var_content_code["527"] = "Railgun Error";
}