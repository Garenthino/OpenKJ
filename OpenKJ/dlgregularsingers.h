/*
 * Copyright (c) 2013-2019 Thomas Isaac Lightburn
 *
 *
 * This file is part of OpenKJ.
 *
 * OpenKJ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef REGULARSINGERSDIALOG_H
#define REGULARSINGERSDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include <QSqlTableModel>
#include "regitemdelegate.h"
#include "rotationmodel.h"

namespace Ui {
class DlgRegularSingers;
}

class RegProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RegProxyModel(QObject *parent = nullptr);
    void setFilterString(const QString &value);
private:
    QString filterString;
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

class DlgRegularSingers : public QDialog
{
    Q_OBJECT

private:
    int m_rtClickRegSingerId;
    Ui::DlgRegularSingers *ui;
    QSqlTableModel *regModel;
    RegProxyModel *proxyModel;
    RegItemDelegate *regDelegate;
    RotationModel *rotModel;


    public:
    explicit DlgRegularSingers(RotationModel *rotationModel, QWidget *parent = 0);
    ~DlgRegularSingers();

signals:
    void regularSingerDeleted(int regularID);
    void regularSingerRenamed(int regularID, QString newName);    

private slots:
    void on_btnClose_clicked();
    void on_tableViewRegulars_clicked(const QModelIndex &index);
    void on_tableViewRegulars_customContextMenuRequested(const QPoint &pos);
    void editSingerDuplicateError();
    void renameRegSinger();

    void on_lineEditSearch_textChanged(const QString &arg1);

public slots:
    void regularsChanged();

};

#endif // REGULARSINGERSDIALOG_H
