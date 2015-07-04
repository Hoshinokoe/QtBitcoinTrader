//  This file is part of Qt Bitcion Trader
//      https://github.com/JulyIGHOR/QtBitcoinTrader
//  Copyright (C) 2013-2015 July IGHOR <julyighor@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  In addition, as a special exception, the copyright holders give
//  permission to link the code of portions of this program with the
//  OpenSSL library under certain conditions as described in each
//  individual source file, and distribute linked combinations including
//  the two.
//
//  You must obey the GNU General Public License in all respects for all
//  of the code used other than OpenSSL. If you modify file(s) with this
//  exception, you may extend this exception to your version of the
//  file(s), but you are not obligated to do so. If you do not wish to do
//  so, delete this exception statement from your version. If you delete
//  this exception statement from all source files in the program, then
//  also delete it here.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef JULYHTTP_H
#define JULYHTTP_H

#include <QObject>
#include <QSslSocket>
#include <QTime>
#include <QNetworkCookie>

struct PacketItem
{
	QByteArray *data;
	int reqType;
	int retryCount;
	bool skipOnce;
};

class JulyHttp : public QSslSocket
{
	Q_OBJECT

public:
    bool noReconnect;
	uint getCurrentPacketContentLength(){return contentLength;}
	void clearPendingData();
	void reConnect(bool mastAbort=true);
	bool isReqTypePending(int);
	void sendData(int reqType, const QByteArray &method, QByteArray postData=0, const QByteArray &restSignLine=0, const int &forceRetryCount=-1);

	void prepareData(int reqType, const QByteArray &method, QByteArray postData=0, const QByteArray &restSignLine=0, const int &forceRetryCount=-1);
	void prepareDataSend();
	void prepareDataClear();

	JulyHttp(const QString &hostName, const QByteArray &restKeyLine, QObject *parent, const bool &secure=true, const bool &keepAlive=true, const QByteArray &contentType="application/x-www-form-urlencoded");
	~JulyHttp();

    void setPortForced(quint16 port){forcedPort=port;}
private:
    quint16 forcedPort;
    QByteArray outBuffer;
    QByteArray contentTypeLine;
	int noReconnectCount;
	void addSpeedSize(qint64);
	QMap<QByteArray,QByteArray> cookiesMap;
	void saveCookies();
	int apiDownCounter;
	bool secureConnection;
	bool isDataPending;
	void uncompress(QByteArray *data);
	bool contentGzipped;
	QByteArray *currentPendingRequest;
	bool connectionClose;
	int httpState;
	qint64 bytesDone;
	uint contentLength;
	bool waitingReplay;
	bool readingHeader;
	qint64 chunkedSize;

	void abortSocket();
	bool isDisabled;
	QByteArray cookieLine;
	int outGoingPacketsCount;
	void setupSocket();
	bool isSocketConnected();
	void reconnectSocket(bool mastAbort);
	void setApiDown(bool);
	bool apiDownState;
	bool packetIsChunked;
	QByteArray buffer;
	bool nextPacketMastBeSize;
	bool endOfPacket;
	void clearRequest();
	void retryRequest();

	QTime requestTimeOut;
	QList<PacketItem>requestList;
	QMap<int,int> reqTypePending;

	QList<PacketItem>preparedList;

	void takeFirstRequest();
	void takeRequestAt(int);
	QByteArray restKeyLine;
	QByteArray httpHeader;
	QString hostName;
private slots:
	void sslErrorsSlot(const QList<QSslError> &);
	void errorSlot(QAbstractSocket::SocketError);
	void sendPendingData();
	void readSocket();
signals:
	void setDataPending(bool);
	void dataProgress(int);
	void anyDataReceived();
	void errorSignal(QString);
	void sslErrorSignal(const QList<QSslError> &);
	void apiDown(bool);
	void dataReceived(QByteArray,int);
};

#endif // JULYHTTP_H
