#!/usr/bin/python

import ConfigParser


CFG_DIR = "../example.cfg"
	
def getMobilesValues():
    allMobilesValues = []
    config = ConfigParser.ConfigParser()
    config.read(CFG_DIR)    
    mobiles = config.options("Mobiles")
    for m in mobiles:
        mobileValues = (config.get(m, "name"), config.get(m, "enabled"))
        allMobilesValues.append(mobileValues)
	return allMobilesValues
	
def getList():
    config = ConfigParser.ConfigParser()
    config.read(CFG_DIR)
    users = config.options("Users")
    listValues = []
    
    for u in users:
        userValues = (config.get(u, "login"), config.get(u, "enabled"), config.get(u, "mobile"))
        listValues.append(userValues)
    return listValues
