#ifndef CDGFILEREADER_H
#define CDGFILEREADER_H

#include <QString>
#include "cdgimageframe.h"
#include "libCDG.h"

class CdgFileReader
{
public:
    CdgFileReader(const QString &filename);

    /**
     * @brief Read first/next frame from the data stream.
     * @note  Replaces currentFrame() with the next frame with visible changes and
     * sets currentFrameDurationMS() and currentFramePositionMS() as well.
     * @return true if here are more frames to be read. false if EOF.
     */
    bool moveToNextFrame();

    std::array<uchar, cdg::CDG_IMAGE_SIZE> currentFrame() { return m_current_image_data; }
    int currentFrameDurationMS();
    int currentFramePositionMS();

    /**
     * @brief Set currentFrame() to the frame that should be displayed at a given point in time.
     * Note: If the position given is less than the position of currentFrame, the file is "rewinded" and read from the start.
     * @param positionMS The position in milliseconds.
     * @return true is positionMS is within file range.
     */
    bool seek(int positionMS);

    /**
     * @brief Duration of the entire file in milliseconds
     * @return
     */
    int getTotalDurationMS();

    /**
     * Returns the position of the very last frame.
     * This can be less than the total duration, beceause: "total duration = position + duration of final frame".
     *
     * @return The value is not known until all data is read. If so, -1 is returned.
     */
    void scanForFinalDrawPosition();
    int positionOfFinalFrameMS();
    int positionOfFinalDrawMS() const;

#ifdef QT_DEBUG
    void saveNextImgToFile();
    void saveCurrentImgToFile();
#endif

private:
    void rewind();
    bool readAndProcessNextPackage();
    inline bool isEOF();

    inline static int getDurationOfPackagesInMS(const int numberOfPackages);

    QByteArray m_cdgData;
    int m_cdgDataPos;

    std::array<uchar, cdg::CDG_IMAGE_SIZE> m_current_image_data;
    int m_current_image_pgk_idx;

    CdgImageFrame m_next_image;
    int m_next_image_pgk_idx;
    int m_lastDrawPosMs{0};
};

#endif // CDGFILEREADER_H
