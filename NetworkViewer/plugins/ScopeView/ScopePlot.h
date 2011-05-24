/**
     Copyright (C) 2009-2011 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca

     This file is part of OpenECoSys/NetworkViewer.
     OpenECoSys/NetworkViewer is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by the Free Software
     Foundation, either version 3 of the License, or (at your option) any later version.
     OpenECoSys/NetworkViewer is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
     or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
     You should have received a copy of the GNU General Public License along with
     OpenECoSys/NetworkViewer. If not, see http://www.gnu.org/licenses/.

 */

#ifndef _SCOPE_PLOT_H_
#define _SCOPE_PLOT_H_

#include "qwt_plot.h"

class ScopeView;


class ScopePlot : public QwtPlot
{

    Q_OBJECT;

public :

        ScopePlot(ScopeView *parent = NULL);

protected:

    ///Internal drop event handler
    //void dropEvent(QDropEvent *event);
    //void dragEnterEvent(QDragEnterEvent *event);

    ScopeView *m_view;

};


#endif
