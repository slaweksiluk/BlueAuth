#!/usr/bin/env python

from gi.repository import Gtk
#import Gtk
import serviceDiscProt
import BTfunctions
import CryptoEngine
import cfgManage
import addUserWin
import windows
import sys
import bluetooth
from bluetooth import *
import os.path
import string
import random
import hashlib
import time
import base64



class mainWindow(Gtk.Window):
#  Sciezka do katalogu z kluczami
    CONS_UUID = "f893253a-9f4d-11e4-91c3-74e5"
    KEYS_PATH = "../keys/"

    MSG_LEN = 8;

    def __init__(self):
# glowne okno
        print (sys.version)
        Gtk.Window.__init__(self, title="Settings")
        window = Gtk.Window()
        #window.set_default_size(200, 200)
        window.set_border_width(10)

# okno dodaj uzytkownika
        addUserWin = Gtk.Window()
        addUserWin.set_default_size(200, 200)

# Kolumny z ustawieniami uzytkownikow
        rows = cfgManage.getList()

        liststore = Gtk.ListStore(str, bool, str)
        for act in rows:
# tutaj zamiast ID telefonu trzeba dac jego nazwe
            #mobileName = cfgManage.getMobileName(act[2])
            liststore.append([act[0], act[1], act[2]])

        treeview = Gtk.TreeView(liststore)
        column_text = Gtk.TreeViewColumn("Users")
        column_toggle = Gtk.TreeViewColumn("Enabled")
        column_mobiles = Gtk.TreeViewColumn("Mobiles")
        treeview.append_column(column_text)
        treeview.append_column(column_toggle)
        treeview.append_column(column_mobiles)

        cellrenderer_text = Gtk.CellRendererText()
        column_text.pack_start(cellrenderer_text, False)
        column_text.add_attribute(cellrenderer_text, "text", 0)

        cellrenderer_toggle = Gtk.CellRendererToggle()
        column_toggle.pack_start(cellrenderer_toggle, True)
        column_toggle.add_attribute(cellrenderer_toggle, "active", 1)

#        for act in rows:
#            if act[1] == True:
#            cellrenderer_toggle

        cellrenderer_mobiles = Gtk.CellRendererText()
        column_mobiles.pack_start(cellrenderer_mobiles, False)
        column_mobiles.add_attribute(cellrenderer_mobiles, "text", 2)

# wykrycie kliknicie na ktoregos uzytkownika
        selection_users = treeview.get_selection()
        #selection.connect('changed', self.on_treeview_selection_changed)

#  treeView dla listy telefonow
        temp_mobile = cfgManage.getMobilesValues()
        liststore2 = Gtk.ListStore(str, bool, str, str, str)
        for i in temp_mobile:
            liststore2.append(i)

        treeview2 = Gtk.TreeView(liststore2)
        column_1 = Gtk.TreeViewColumn("ID")
        column_2 = Gtk.TreeViewColumn("Enabled")
        column_3 = Gtk.TreeViewColumn("UUID")
        column_4 = Gtk.TreeViewColumn("BlueTooth address")
        column_5 = Gtk.TreeViewColumn("Port")

        treeview2.append_column(column_1)
        treeview2.append_column(column_2)
        treeview2.append_column(column_3)
        treeview2.append_column(column_4)
        treeview2.append_column(column_5)

        cellrenderer_1 = Gtk.CellRendererText()
        column_1.pack_start(cellrenderer_1, False)
        column_1.add_attribute(cellrenderer_1, "text", 0)

        cellrenderer_2 = Gtk.CellRendererToggle()
        column_2.pack_start(cellrenderer_2, True)
        column_2.add_attribute(cellrenderer_2, "active", 1)

        cellrenderer_3 = Gtk.CellRendererText()
        column_3.pack_start(cellrenderer_3, False)
        column_3.add_attribute(cellrenderer_3, "text", 2)

        cellrenderer_4 = Gtk.CellRendererText()
        column_4.pack_start(cellrenderer_4, False)
        column_4.add_attribute(cellrenderer_4, "text", 3)

        cellrenderer_5 = Gtk.CellRendererText()
        column_5.pack_start(cellrenderer_5, False)
        column_5.add_attribute(cellrenderer_5, "text", 4)

# wykrycie kliknieca na ktorys mobile
        selection_mobiles = treeview2.get_selection()
# podlaczanie sygnalow
        window.connect("destroy", lambda w: Gtk.main_quit())
# fajka enabled dla usera
        cellrenderer_toggle.connect("toggled", self.cell_toggled, liststore)
# fajka dla mobiles
        cellrenderer_2.connect("toggled", self.cell_toggled_mobiles,
                               liststore2)

# przyciski
        button1 = Gtk.Button("Add user")
        button2 = Gtk.Button("Delete user")

        button3 = Gtk.Button("Add mobile")
        button4 = Gtk.Button("Delete mobile")
        button5 = Gtk.Button("Update mobile")
        button6 = Gtk.Button("Save config")

# sygnaly do przyciskow
        button1.connect("clicked", self.on_Add_user_clicked, liststore,
                        liststore2)
        button2.connect("clicked", self.delete_row,
                        selection_users)

        button3.connect("clicked", self.on_Add_mobile_clicked,
                        liststore2)
        button4.connect("clicked", self.delete_row, selection_mobiles)
        button5.connect("clicked", self.on_update_mobile_clicked,
                        selection_mobiles)
        button6.connect("clicked", self.on_Save_config_clicked, liststore2,
                        liststore)

# box dla przycikow pod users i mobiles
        usersButtonsBox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,
                                  spacing=6)
        usersButtonsBox.pack_start(button1, True, True, 0)
        usersButtonsBox.pack_start(button2, True, True, 0)

        mobilesButtonsBox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,
                                    spacing=6)
        mobilesButtonsBox.pack_start(button3, True, True, 0)
        mobilesButtonsBox.pack_start(button4, True, True, 0)
        mobilesButtonsBox.pack_start(button5, True, True, 0)
        mobilesButtonsBox.pack_start(button6, True, True, 0)

# szkielet
        grid = Gtk.Table(3, 2, False)
        grid.attach(treeview, 0, 1, 0, 1)
        grid.attach(usersButtonsBox, 0, 1, 1, 2)
        #grid.attach(button2, 0, 1, 2, 3)

        grid.attach(treeview2, 1, 2, 0, 1)
        grid.attach(mobilesButtonsBox, 1, 2, 1, 2)
        #grid.attach(button4, 1, 2, 2, 3)

        window.add(grid)
        #window.add(treeview)
        window.show_all()

# funkcja do oblusgi ptaszka dla users
    def cell_toggled(self, widget, path, model):
        model[path][1] = not model[path][1]
        itr = model.get_iter(path)
        val = model.get_value(itr, 1)
        print "Liststore: " + str(val)

# funkcja do oblusgi ptaszka dla mobiles
    def cell_toggled_mobiles(self, widget, path, store):
        if (store[path][3] == None) or (store[path][4] == None):
            itr = store.get_iter(path)
            val = store.get_value(itr, 0)
            dialog = Gtk.MessageDialog(self, 0, Gtk.MessageType.ERROR,
                Gtk.ButtonsType.CANCEL, "Unable to enable phone: " + str(val))
            dialog.format_secondary_text(
"Please update the phone, by choosing Update mobile button before enabling it")
            dialog.run()

            dialog.destroy()
        else:
            store[path][1] = not store[path][1]
            itr = store.get_iter(path)
            val = store.get_value(itr, 1)
            print "Liststore mobiles: " + str(val)

# okienko z dodaniem uzytkownika
    def on_Add_user_clicked(self, obj, liststore, liststore2):
        addUserWin.addUserWin(liststore, liststore2)
#        newWindow.show_all()
#        while (newWindow.ready_check() == False):
#            time.sleep(5)
#            print "wait for user"
        #liststore.append(newWindow.userTuple)

# usuniecie zaznacczonego wiersza
    def delete_row(self, obj, tree_selection):
        (model, pathlist) = tree_selection.get_selected_rows()
        tree_iter = model.get_iter(pathlist)
        if(model.remove(tree_iter)):
            print "removed"
        else:
            print "err"

# dodanie telefonu
    def on_Add_mobile_clicked(self, widget, liststore):
# urucuchominie skryptu do wygenerowania pliku JAR z UUID i wyswietlenie
# okna z prosba o zapisanie ytego pliku w dowolnym miejscu
# Wygeneruj 4 cyfrowy numer do utworzenia UUID w telfonie

#        uuid_part = ''.join(random.choice("0123456789abcdef") for _ in range(4))
#        serviceDiscProt.findMobile(uuid_part)
#
#        dialog = Gtk.MessageDialog(self, 0, Gtk.MessageType.INFO,
#            Gtk.ButtonsType.CANCEL, "Please pass this numer to mobile")
#        dialog.format_secondary_text(uuid_part)
#        dialog.run()
#        print("INFO dialog closed")

#        dialog.destroy()

# Lepiej bedzie generowac UUID na telefonie i przepisac jego czesc do komputera
        uuid_suffix = windows.entryWindow(self, "Enter id presented by mobile",
                                    "Add mobile")
        (btaddr, port) = serviceDiscProt.findPhone(mainWindow.CONS_UUID + uuid_suffix)
        # uuid = manageJAR.createJAR()

        # Znaleziono telefon, trzbeba wygenrowac pare kluczy
        pubKey = CryptoEngie.genKeys(KEYS_PATH + "pub.pem",
                                     KEYS_PATH + "priv.pem")


        # Popros uzywkonika o nazwe telefonu
        entry = windows.entryWindow(self, "Phone found! Enter its name",
                                    "Add mobile")



# dopoki nie polaczenie z telefonem nie zostanie zrealizowane i
# nie zosanie pobrany btaddr i port sa puste i telefon jest domyslni
# wylaczony
        liststore.append((entry, False, uuid, None, None))



    def on_update_mobile_clicked(self, widget, tree_selection):
        result = ()
        (model, pathlist) = tree_selection.get_selected_rows()
        tree_iter = model.get_iter(pathlist)

# tu trzeba znalesc telefon o okreslonym UUID
        uuid = model[tree_iter][2]
        mobile = model[tree_iter][1]

        try:
            (btaddr, port) = serviceDiscProt.findPhone(uuid)
        except ValueError as err:
            print(err.args)
            dialog = Gtk.MessageDialog(self, 0, Gtk.MessageType.ERROR, Gtk.ButtonsType.CANCEL, "Unable to find phone")
            dialog.format_secondary_text("Unable to find phone with service UUID : " + str(uuid) + " Please chceck if its properly set.")
            dialog.run()
            dialog.destroy()
            return None;

#  Trzeba wygenerowac i wyslac klucz publiczny... Tylko jesli jest to pierwsze
            #  uruchomienie Update Mobile. Bedzie to sprawdzane w konstruktorze
            #  klasy crytpoRSA. Jesli plik keys/<mobile> istnieje znaczy ze
            #  klucz jest juz okresolny. Trzeba tylko sprawdzic polaczenie.
            if not os.path.exists(KEYS_PATH + mobile):
#  Trzeba:
            #  1. wygenerowac klucze
                key_len = 1024  # Minimum 1024 bity
                cryptoRSA.generateKeys(key_len)
            #  2. Wyslac do telefonu dl. klucza i sam klucz
                sendKey()





# jesli pole key jest puste to trzeba je wyslac do mobile
        key_s = "f893253a9f4d11e2"  # 16 znakow, na sztywno
        try:
            sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
            sock.connect((btaddr, port))
            sock.send("KEY_128")
            sock.send(key_s)
            sock.close()
            print "Key sent"
        except BluetoothError as e:
            print "BT error: " + e.errno + e.strerror
            sock.close()

# i zapisac te dane to listy
        model[tree_iter][3] = btaddr
        model[tree_iter][4] = str(port)
        print "Adrr: " + str(btaddr) + " Port:" + str(port)

    def on_Save_config_clicked(self, widget, mobiles_liststore,
                               users_liststore):
        cfgManage.saveMobiles(mobiles_liststore)
        cfgManage.saveUsers(users_liststore)

    def sendKey(btaddr, port):
        try:
            sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
            sock.connect((btaddr, port))
            sock.send("KEY1024") # klucz 1024bitowy
            sock.close()
            print "connTest(): Key Req sent"

            data = ""
            for i in range(1, KEY_CHARS+1):
                ch = sock.recv(1)
                data = data + ch
            print "connTest():received [%s]" % data
            sock.close()
        except BluetoothError as e:
            print "connTest(): BT error: " + e.errno + e.strerror
            sock.close()

    def sendMsg(btaddr, port, msg):
        try:
            sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
            sock.connect((btaddr, port))
            sock.send(msg) # klucz 1024bitowy
            sock.close()
            print "connTest(): AUTH_REQ sent"

            data = ""
            for i in range(1, KEY_CHARS+1):
                ch = sock.recv(1)
                data = data + ch
            print "connTest():received [%s]" % data
            sock.close()
        except BluetoothError as e:
            print "connTest(): BT error: " + e.errno + e.strerror
            sock.close()


    def connTest(btaddr, port):
        try:
            sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
            sock.connect((btaddr, port))
            sock.send("KEY_REQ")
            sock.close()
            print "connTest(): Key Req sent"

            data = ""
            for i in range(1, KEY_CHARS+1):
                ch = sock.recv(1)
                data = data + ch
            print "connTest():received [%s]" % data
            sock.close()
        except BluetoothError as e:
            print "connTest(): BT error: " + e.errno + e.strerror
            sock.close()

def configureMobile(addr, port, uuid_suf, pub_key, sessionID_B64, priv_key_dir):
    if addr == None:
        (addr, port) = BTfunctions.findPhone(mainWindow.CONS_UUID +
            uuid_suf)
    BTfunctions.sendPubKey(addr, port, pub_key)

    # Odczekaj 3 s...
    print "sleep 10s..."
    time.sleep(10)
    ct = BTfunctions.receiveCipherText(addr, port, sessionID_B64)
    decrypted = CryptoEngine.decryptRSA(ct, priv_key_dir, True, True)

    rec_sessionID = decrypted[0:7]
    if base64encode(rec_sessionID) != sessionID_B64:
        print "ERROR: diffrents sesion IDs!"
        return None
    rec_keyID = decrypted[8:39]

    key_id_hash_b64 = base64.b64encode(hashlib.sha512(rec_keyID))
    print "Hashed key id: "
    print key_id_hash_b64
    return key_id_hash_b64


pub_dir = "../keys/pub.pem"
pub_file = open(pub_dir, 'r')
pub_key = pub_file.read()
pub_file.close()
pub_key_temp = pub_key.replace("-----BEGIN PUBLIC KEY-----\n", "")
pub_key_to_send = pub_key_temp.replace("-----END PUBLIC KEY-----", "").rstrip()

print "pub key:"
print pub_key_to_send
print "end pub key"

sessionID_B64 = "mEqdOVZNd8Y="
priv_key_dir = "../keys/priv.pem"

addr = "8C:71:F8:99:F0:73"
port = 8

configureMobile(addr, port, "0b44a938", pub_key_to_send, sessionID_B64, priv_key_dir)
#configureMobile2("8C:71:F8:99:F0:73", 8, sessionID_B64, priv_key_dir)

#mainWindow()
#Gtk.main()
