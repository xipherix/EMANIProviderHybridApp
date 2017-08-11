# Overview
This example demonstrates how to use Elektron Message API C++ (EMA C++) consuming data and re-publishing the data to ADH cache for loacal or internal publshing to Thomson Reuters Market Data system.

# Installation
Define the project environment variables
## Windows VS2013
set environment variable name EMAInstall to  <EMA install dir> before open the solution on visual studio.
Eg. set EMAInstall="C:\Projects\API\EMA"

## Linux
Open makefile and change EMAInstall to <EMA install dir>
export EMAInstall="/home/Projects/API/EMA"

##Usage
```
EMA Consumer Non Interactive Provider Hybrid demo
Usage:
The following options are supported

 -s       Followed by the subscribing service name
 -nip     Followed by the non-interactive publishing service name
 -t       Followed by a list of item names seperated by space. Required by consumer and non-iteractive client publish

 -tf      Followed absolute file to filename contains item list
          Note: command line options can contains either -t or -tf. If it contains both options it will use item list from last one

 Optional options

 -nipuser Followed by Username for Non interactive provider, default is username

 -u       Followed by Username, default is username
 -pos     Followed by Position, default is 127.0.0.1/net
 -svcid   Followed by Service ID for Non Interactive publishing service, default is 8500 -appid Foolowed by Application ID, default is 256
 -v       Print OMM Refresh and Update message to console output
 -runtime Followed by Application runtime period in second, default is 600 second

For example:
        emahybridapp -s DIRECT_FEED -nip NIPUB -u username -t IBM.N CSCO.O
        or
        emahybridapp -s DIRECT_FEED -nip NIPUB -u username -tf ./itemlist.txt
        
	Creates a service called NIPUB which derives all its items from DIRECT_FEED

        emahybridapp.exe -s DIRECT_FEED -nip NIPUB -u username -t IBM.N -runtime 3600
        
	For running the application 3600 second and then exit

The Demo application works in pass through mode.
It will re-publish the data as is based on the data from item list user provided
```


