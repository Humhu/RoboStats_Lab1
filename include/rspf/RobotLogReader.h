#ifndef _ROBOT_LOG_READER_H_
#define _ROBOT_LOG_READER_H_

#include "rspf/PoseSE2.h"
#include "rspf/Parameterized.h"

#include <fstream>
#include <array>
#include <math.h>

namespace rspf {

    // NOTE Convention is x-forward, left-y robot frame
    // NOTE Units are cm and radians
    struct SensorData {
        static const unsigned int ScanSize = 180;
        static constexpr double StartAngle = -M_PI/2.0;
        static constexpr double EndAngle = M_PI/2.0;
        static constexpr double ScanResolution = M_PI/ScanSize;
		static constexpr double MaxRange = 818.3;
        
        typedef std::array<double, ScanSize> Scan;

        const PoseSE2 displacement;
        const double timestamp;
        const bool hasScan;
        const Scan points;
        const PoseSE2 laserOffset; // Offset of laser in robot frame

        SensorData( const PoseSE2& disp, double time );
        SensorData( const PoseSE2& disp, double time,
                    const Scan& pts, const PoseSE2& laserOff );
    };

    std::ostream& operator<<( std::ostream& os, const SensorData& data );
    
    class RobotLogReader {
    public:

        /*! \brief Opens the specified file for log reading. */
        RobotLogReader( const std::string& filename );

		/*! \brief Parses a ptree to find the filepath. */
		RobotLogReader( const PropertyTree& ptree );

        /*! \brief Returns if there is more data. */
        bool HasData() const;
        
        /*! \brief Returns the next data line. */
        SensorData GetNextData();

    protected:

        bool positionInitialized;
		double scale; // Scale for translations
        PoseSE2 lastPosition;
        std::string currentLine;

		std::string logPath;
        std::ifstream logFile;

		void Initialize();

    };
    
}

#endif