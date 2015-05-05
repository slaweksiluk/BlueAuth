#!/usr/bin/python

import ConfigParser


USERS_CFG_DIR = "../users.cfg"
MOBILES_CFG_DIR = "../mobiles.cfg"


def getMobilesValues():
    config = ConfigParser.ConfigParser()
    config.read(MOBILES_CFG_DIR)
    mobiles = config.sections()

    allMobilesValues = []
    for u in mobiles:
        mobileValues = (u,
                        config.get(u, "enabled"),
                        config.get(u, "uuid"),
                        config.get(u, "btaddr"),
                        config.get(u, "port"))
        allMobilesValues.append(mobileValues)
    return allMobilesValues
    
def get


def getList():
    config = ConfigParser.ConfigParser()
    config.read(USERS_CFG_DIR)
    users = config.sections()
    listValues = []

    for u in users:
        userValues = (u, config.get(u, "enabled"),
                      config.get(u, "mobile"))
        listValues.append(userValues)
    return listValues


def getMobileName(mobile):
    config = ConfigParser.ConfigParser()
    config.read(MOBILES_CFG_DIR)
    return config.get(mobile, "name")


def getMobilesNames():
    config = ConfigParser.ConfigParser()
    config.read(MOBILES_CFG_DIR)
    mobiles = config.sections()

#    allMobilesValues = []
#    for u in mobiles:
#        mobileValues = (config.get(u, "name"))
#        allMobilesValues.append(mobileValues)
    return mobiles

######################################################
# Zapis do configu
######################################################
#def setUUID(mobile, uuid):
#    config = ConfigParser.ConfigParser()
#    config.read(MOBILES_CFG_DIR)
#    config.set(mobile, "uuid", uuid)


def saveMobiles(mobiles_liststore):
    config = ConfigParser.ConfigParser()
    config.read(MOBILES_CFG_DIR)

    for row in mobiles_liststore:
        ID = row[0]
        enabled = row[1]
        UUID = row[2]
        btaddr = row[3]
        port = row[4]
# czy jest juz taki telefon w w  pliku?
        if not config.has_section(ID):
            config.add_section(ID)
        config.set(ID, "enabled", enabled)
        config.set(ID, "uuid", UUID)
        config.set(ID, "btaddr", btaddr)
        config.set(ID, "port", port)

    with open(MOBILES_CFG_DIR, 'wb') as configfile:
        config.write(configfile)
# usun sekcje
    delete_sections(MOBILES_CFG_DIR, mobiles_liststore)


def saveUsers(users_liststore):
    config = ConfigParser.ConfigParser()
    config.read(USERS_CFG_DIR)

    for row in users_liststore:
        ID = row[0]
        enabled = row[1]
        mobile = row[2]
# czy jest juz taki telefon w w  pliku?
        if not config.has_section(ID):
            config.add_section(ID)

        config.set(ID, "enabled", enabled)
        config.set(ID, "mobile", mobile)

    with open(USERS_CFG_DIR, 'wb') as configfile:
            config.write(configfile)
# usun sekcje
    delete_sections(USERS_CFG_DIR, users_liststore)


# czy w pliku sa sekcje ktorych nie ma w gui?
def delete_sections(CFG_DIR, store):
    config = ConfigParser.ConfigParser()
    config.read(CFG_DIR)
    sections = config.sections()
    for s in sections:
        for row in store:
            ID = row[0]
            if s == ID:
                flag = True
                break
            else:
                flag = False
        if not flag:
            config.remove_section(s)
# zapisz do pliku
    with open(CFG_DIR, 'wb') as configfile:
        config.write(configfile)
