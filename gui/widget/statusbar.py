# DFF -- An Open Source Digital Forensics Framework
# Copyright (C) 2009-2013 ArxSys
# This program is free software, distributed under the terms of
# the GNU General Public License Version 2. See the LICENSE file
# at the top of the source tree.
#  
# See http://www.digital-forensic.org for more information about this
# project. Please do not directly contact any of the maintainers of
# DFF for assistance; the project provides a web site, mailing lists
# and IRC channels for your use.
# 
# Author(s):
#  Frederic Baguelin <fba@digital-forensic.org>
# 

from dff.ui.gui.resources.ui_status import Ui_StatusBar
from PyQt4.QtGui import QWidget
from PyQt4.QtCore import QString

class StatusBarWidget(Ui_StatusBar, QWidget):
    def __init__(self, parent = None):
        super(QWidget, self).__init__(parent)
        self.setupUi(self)


    def updateNodesStatus(self, regular, deleted):
        status = QString(str(regular + deleted))
        status += " (" + str(regular) + "+"
        status += "<font color=red>" + str(deleted) + "</font>"  + ")"
        self.nodesStatus.setText(status)


    def updateFilesStatus(self, regular, deleted):
        status = QString(str(regular + deleted))
        status += " (" + str(regular) + "+"
        status += "<font color=red>" + str(deleted) + "</font>"  + ")"
        self.filesStatus.setText(status)
        
    def updateFoldersStatus(self, regular, deleted):
        status = QString(str(regular + deleted))
        status += " (" + str(regular) + "+"
        status += "<font color=red>" + str(deleted) + "</font>" + ")"
        self.foldersStatus.setText(status)

        
    def updateSelectedStatus(self, selected):
        self.selectedStatus.setText(str(selected))


    def updateBytesStatus(self, tbytes):
        self.totalBytesCount.setText(str(tbytes))


    def updateStatus(self, currentViewStatus, selected):
        self.updateNodesStatus(currentViewStatus.regularNodes(), currentViewStatus.deletedNodes())
        self.updateFilesStatus(currentViewStatus.regularFiles(), currentViewStatus.deletedFiles())
        self.updateFoldersStatus(currentViewStatus.regularFolders(), currentViewStatus.deletedFolders())
        self.updateBytesStatus(currentViewStatus.totalBytes())
        self.updateSelectedStatus(selected)
