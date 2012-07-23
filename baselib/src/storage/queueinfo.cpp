/* XiVO Client
 * Copyright (C) 2007-2011, Avencall
 *
 * This file is part of XiVO Client.
 *
 * XiVO Client is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version, with a Section 7 Additional
 * Permission as follows:
 *   This notice constitutes a grant of such permission as is necessary
 *   to combine or link this software, or a modified version of it, with
 *   the OpenSSL project's "OpenSSL" library, or a derivative work of it,
 *   and to copy, modify, and distribute the resulting work. This is an
 *   extension of the special permission given by Trolltech to link the
 *   Qt code with the OpenSSL library (see
 *   <http://doc.trolltech.com/4.4/gpl.html>). The OpenSSL library is
 *   licensed under a dual license: the OpenSSL License and the original
 *   SSLeay license.
 *
 * XiVO Client is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XiVO Client.  If not, see <http://www.gnu.org/licenses/>.
 */

/* $Revision$
 * $Date$
 */

#include <QDebug>
#include "queueinfo.h"

QueueInfo::QueueInfo(const QString & ipbxid,
                     const QString & id)
    : XInfo(ipbxid, id)
{
}

bool QueueInfo::updateConfig(const QVariantMap & prop)
{
    bool haschanged = false;
    haschanged |= setIfChangeString(prop, "context", & m_context);
    haschanged |= setIfChangeString(prop, "name", & m_name);
    haschanged |= setIfChangeString(prop, "displayname", & m_displayname);
    haschanged |= setIfChangeString(prop, "number", & m_number);
    return haschanged;
}

bool QueueInfo::updateStatus(const QVariantMap & prop)
{
    bool haschanged = false;
    if (prop.contains("agentmembers")) {
        m_xagentids.clear();
        m_agentmembers.clear();
        foreach (QString agentid, prop.value("agentmembers").toStringList()) {
            QString xagentid = QString("%1/%2").arg(m_ipbxid).arg(agentid); // to match against agent membership
            QString agentmember = QString("qa:%1-%2").arg(m_id).arg(agentid); // for requests to server
            m_xagentids.append(xagentid);
            m_agentmembers.append(agentmember);
        }
        haschanged = true;
    }
    if (prop.contains("phonemembers")) {
        m_xphoneids.clear();
        m_phonemembers.clear();
        foreach (QString phoneid, prop.value("phonemembers").toStringList()) {
            QString xphoneid = QString("%1/%2").arg(m_ipbxid).arg(phoneid); // to match against phone membership
            QString phonemember = QString("qp:%1-%2").arg(m_id).arg(phoneid); // for requests to server
            m_xphoneids.append(xphoneid);
            m_phonemembers.append(phonemember);
        }
        haschanged = true;
    }
    return haschanged;
}

const QString & QueueInfo::context() const
{
    return m_context;
}

const QString & QueueInfo::queueNumber() const
{
    return m_number;
}

const QString & QueueInfo::queueName() const
{
    return m_name;
}

const QString & QueueInfo::queueDisplayName() const
{
    return m_displayname;
}

const QStringList & QueueInfo::xagentids() const
{
    return m_xagentids;
}

const QStringList & QueueInfo::agentmembers() const
{
    return m_agentmembers;
}

const QStringList & QueueInfo::xphoneids() const
{
    return m_xphoneids;
}

const QStringList & QueueInfo::phonemembers() const
{
    return m_phonemembers;
}

QString QueueInfo::reference(const QString & where, const QString & xmemberid) const
{
    QString ref = "";
    if (where == "agents") {
        if (m_xagentids.contains(xmemberid)) {
            ref = xmemberid;
            ref.replace("/", QString("/qa:%1-").arg(id()));
        }
    } else if (where == "phones") {
        if (m_xphoneids.contains(xmemberid)) {
            ref = xmemberid;
            ref.replace("/", QString("/qp:%1-").arg(id()));
        }
    }
    return ref;
}
