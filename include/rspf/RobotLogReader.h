#ifndef _ROBOT_LOG_READER_H_
#define _ROBOT_LOG_READER_H_

#include "rspf/PoseSE2.h"

#include <fstream>
#include <array>

namespace rspf {

    // NOTE Units are mm and radians
    struct SensorData {
        static const unsigned int ScanSize = 180;
        typedef std::array<double, ScanSize> Scan;
        
        const PoseSE2 displacement;
        const double timestamp;
        const bool hasScan;
        const Scan points;

        SensorData( const PoseSE2& disp, double time );
        SensorData( const PoseSE2& disp, double time, const Scan& pts );
    };

    std::ostream& operator<<( std::ostream& os, const SensorData& data );
    
    class RobotLogReader {
    public:

        /*! \brief Opens the specified file for log reading. */
        RobotLogReader( const std::string& filename );

        /*! \brief Returns if there is more data. */
        bool HasData() const;
        
        /*! \brief Returns the next data line. */
        SensorData GetNextData();

    protected:

        bool positionInitialized;
        PoseSE2 lastPosition;
        std::string currentLine;
        
        std::ifstream logFile;

    };
    
}

#endif