#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QMultiMap>

const int   CITY_COLUMN     = 14;
const int   COUNTY_COLUMN   = 15;
const int   E5_COLUMN       = 19;
const int   E10_COLUMN      = 20;
const int   B7P_COLUMN      = 21;
const int   B7S_COLUMN      = 22;
const char  COUNTY_NAME[]   = "DORSET";

struct displayValue
{
    QString city;
    QString county;
    QString e5;
    QString e10;
    QString b7p;
    QString b7s;
};

struct displayValues
{
    int maxCity;
    int maxCounty;
    int maxE5;
    int maxE10;
    int maxB7p;
    int maxB7s;
    QMultiMap<float, struct displayValue>   values;
};

struct displayValues    dvs
{
    0,
    0,
    0,
    0,
    0,
    0,
    QMultiMap<float, struct displayValue>()
};

int main(int argc, char *argv[])
{
    QCoreApplication    a(argc, argv);
    QTextStream         ts(stdout);
    int                 result = 0;

    while (true)
    {
        if (argc == 1)
        {
            ts << "Usage: MyLocalFuelprices <path to csv file>" << Qt::endl;
            ts.flush();
            result = -2;
            break;
        }

        QString fuelFilePath = QCoreApplication::arguments().at(1);
        QFile   fuelFile(fuelFilePath);

        if (fuelFile.open(QIODeviceBase::ReadOnly) == true)
        {
            QStringList fuelFileLines;
            // Read in file contents one line at a time
            while(!fuelFile.atEnd())
            {
                QByteArray  ba = fuelFile.readLine();
                fuelFileLines.append(ba);
            }
            fuelFile.close();

            // Now search through the lines looking for ones of interest
            // Note the first line is the contains the column headers
            for (int i = 1; i < fuelFileLines.size(); i++)
            {
                QStringList columns;
                columns = fuelFileLines.at(i).split(",", Qt::KeepEmptyParts);
                if (columns.at(COUNTY_COLUMN) == COUNTY_NAME)
                {
                    ts << i
                       << " "
                       << columns.at(CITY_COLUMN)
                       << " "
                       << columns.at(COUNTY_COLUMN)
                       << " "
                       << columns.at(E5_COLUMN)
                       << " "
                       << columns.at(E10_COLUMN)
                       << " "
                       << columns.at(B7P_COLUMN)
                       << " "
                       << columns.at(B7S_COLUMN)
                       << " "
                       << Qt::endl;
                    ts.flush();
                }
            }
            break;
        }

        ts << "Failed to open file '" << fuelFilePath << "'" << Qt::endl;
        ts.flush();
        result = -1;
        break;
    }

    return result;
}
