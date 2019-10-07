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

#include "khaudiorecorder.h"
#include <QAudioEncoderSettings>
#include <QUrl>
#include <QDir>
#include "settings.h"
#include <QDebug>
#include <QDateTime>

extern Settings *settings;

KhAudioRecorder::KhAudioRecorder(QObject *parent) :
    QObject(parent)
{
    audioRecorder = new QAudioRecorder(this);
    connect(audioRecorder, SIGNAL(error(QMediaRecorder::Error)), this, SLOT(audioRecorderError(QMediaRecorder::Error)));
    outputFile = "";
    startDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd-hhmm");
}

void KhAudioRecorder::record(QString filename)
{
    QString outputDir = settings->recordingOutputDir() + QDir::separator() + "Karaoke Recordings" + QDir::separator() + "Show Beginning " + startDateTime;
    QDir dir;
    QString outputFilePath;
    if (settings->recordingContainer() == "raw")
        outputFilePath = outputDir + QDir::separator() + filename + "." + settings->recordingRawExtension();
    else
        outputFilePath = outputDir + QDir::separator() + filename + "." + settings->recordingContainer();
    dir.mkpath(outputDir);
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec(settings->recordingCodec());
    audioSettings.setQuality(QMultimedia::HighQuality);
    audioRecorder->setAudioInput(settings->recordingInput());
    audioRecorder->setEncodingSettings(audioSettings);
    audioRecorder->setContainerFormat(settings->recordingContainer());
    audioRecorder->setOutputLocation(QUrl(outputFilePath));
    audioRecorder->setVolume(1.0);
    audioRecorder->record();
}

void KhAudioRecorder::stop()
{
    if (audioRecorder->state() == QMediaRecorder::RecordingState)
    {
        audioRecorder->stop();
    }
}

void KhAudioRecorder::pause()
{
    qDebug() << "KhAudioRecorder::pause() called";
    if (audioRecorder->state() == QMediaRecorder::RecordingState)
    {
        audioRecorder->pause();
    }
}

void KhAudioRecorder::unpause()
{
    qDebug() << "KhAudioRecorder::unpause() called";
    if (audioRecorder->state() == QMediaRecorder::PausedState)
    {
        audioRecorder->record();
    }
}

void KhAudioRecorder::audioRecorderError(QMediaRecorder::Error error)
{
    Q_UNUSED(error);
    qCritical() << "QAudioRecorder error: " << audioRecorder->errorString();
}
