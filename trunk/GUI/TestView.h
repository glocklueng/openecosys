/*
 * interArmView.h
 *
 *  Created on: 06.11.2009
 *      Author: wids2301
 */

#ifndef TESTVIEW_H_
#define TESTVIEW_H_

#include "ui_TestView.h"
/*#include <QInputDialog>
#include <limits.h>
#include <QTime>
#include <qwt_plot_curve.h>
#include <vector>
*/

class TestView : public QWidget
{
	Q_OBJECT;

	public:
		TestView(QWidget *parent= NULL);
		virtual ~TestView();


	protected:
		Ui::TestView ui;
};

#endif /* TESTVIEW_H_ */

