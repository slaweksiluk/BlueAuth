# -*- coding: utf-8 -*-
"""
Created on Tue Jan  6 14:50:13 2015

@author: heniek
"""
from gi.repository import Gtk
import pwd  # do listy uzytkownikow
import cfgManage  # pobranie listy telefonow


class addMobileWin(Gtk.Window):

    def __init__(self, usersListstore):
        Gtk.Window.__init__(self, title="Add mobile")
# okno dodaj uzytkownika
        window = Gtk.Window()
        #window.set_default_size(200, 200)

# box
        hbox = Gtk.Box(spacing=6)
        window.add(hbox)
# list box
        listbox = Gtk.ListBox()
        listbox.set_selection_mode(Gtk.SelectionMode.NONE)
        hbox.pack_start(listbox, True, True, 0)

        row = Gtk.ListBoxRow()
        hbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=50)
        row.add(hbox)
        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        hbox.pack_start(vbox, True, True, 0)
# dodanie uzytkownikow do lisyu rozwijanej
        row = Gtk.ListBoxRow()
        hbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=50)
        row.add(hbox)
        label = Gtk.Label("Choose user", xalign=0)

        self.combo = Gtk.ComboBoxText()
        usersList = self.getUsersList()
        for u in usersList:
            self.combo.append_text(u)

        hbox.pack_start(label, True, True, 0)
        hbox.pack_start(self.combo, False, True, 0)
        listbox.add(row)
# dodanie telefonow do list rozwijanej
        row = Gtk.ListBoxRow()
        hbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=50)
        row.add(hbox)
        label = Gtk.Label("Choose mobile", xalign=0)

        self.combo_mobiles = Gtk.ComboBoxText()
        mobiles = cfgManage.getMobilesNames()
        for m in mobiles:
            self.combo_mobiles.append_text(m)

        hbox.pack_start(label, True, True, 0)
        hbox.pack_start(self.combo_mobiles, False, True, 0)
        listbox.add(row)

# przycisk OK
        row = Gtk.ListBoxRow()
        hbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=50)
        row.add(hbox)

        button1 = Gtk.Button("OK")

        hbox.pack_start(button1, False, False, 0)
        listbox.add(row)
# podlaczenie sygnalow
        window.connect("destroy", self.closeWindow)
        button1.connect("clicked", self.on_button1_cliked, self.combo_mobiles,
                        self.combo, usersListstore, window)
# wyswietl okno
        window.show_all()

# rekacja na klikniecie OK
    def on_button1_cliked(self, obj, combo_mobiles, combo_users,
                          usersListstore, win):
        mobile = combo_mobiles.get_active_text()
        login = combo_users.get_active_text()
        userTuple = (login, True, mobile)
        if not (mobile and login):
            print "dodaj uzytkownikow"
        else:
            usersListstore.append(userTuple)
            win.destroy()

# pobranie uzytkownikow w sys
    def getUsersList(self):
        usersList = []
        for p in pwd.getpwall():
            usersList.append(p[0])
        return usersList

# zamykanie
    def closeWindow(self, obj):
        obj.destroy()
