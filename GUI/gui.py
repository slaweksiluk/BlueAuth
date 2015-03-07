#!/usr/bin/env python

from gi.repository import Gtk
#import Gtk
import serviceDiscProt
import cfgManage
import addUserWin
import manageJAR
import windows
import sys
import bluetooth
from bluetooth import *




class mainWindow(Gtk.Window):
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
        uuid = manageJAR.createJAR()
        entry = windows.entryWindow(self, "Enter your mobile name",
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
        (btaddr, port) = serviceDiscProt.findPhone(uuid)

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



mainWindow()
Gtk.main()
