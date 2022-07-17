//    This file is part of ComputerExerciseProblems
//    Copyright (C) 2021  IllusionAutumn

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "mytextedit.h"
#include<QKeyEvent>
mytextedit::mytextedit(QWidget* widget):QTextEdit(widget)
{

}

void mytextedit::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        emit press();
            //要处理的事情
    }
    else if(event->key()==Qt::Key_Delete)
    {
        this->clear();
    }
    else
    {
            QTextEdit::keyPressEvent(event);
    }
}
