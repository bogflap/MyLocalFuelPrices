#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QMultiMap>

const int   POST_CODE_COLUMN    = 11;
const int   CITY_COLUMN         = 14;
const int   COUNTY_COLUMN       = 15;
const int   E5_COLUMN           = 19;
const int   E10_COLUMN          = 20;
const int   B7P_COLUMN          = 21;
const int   B7S_COLUMN          = 22;
const char  COUNTY_NAME[]       = "DORSET";

struct displayValue
{
    QString city;
    QString county;
    QString postCode;
    QString e5;
    QString e10;
    QString b7p;
    QString b7s;
};

struct displayValues
{
    qsizetype maxCity;
    qsizetype maxCounty;
    qsizetype maxPostCode;
    qsizetype maxE5;
    qsizetype maxE10;
    qsizetype maxB7p;
    qsizetype maxB7s;
    QMultiMap<float, struct displayValue>   values;
};

void setMaxSize (qsizetype* current, qsizetype newCandidate)
{
    if (*current < newCandidate)
    {
        *current = newCandidate;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication    a(argc, argv);
    QTextStream         ts(stdout);
    int                 result = 0;

    struct displayValues    dvs
        {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            QMultiMap<float, struct displayValue>()
        };

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
            // Note we are omitting any line that does not have a valid E5 value
            for (int i = 1; i < fuelFileLines.size(); i++)
            {
                QStringList columns;
                columns = fuelFileLines.at(i).split(",", Qt::KeepEmptyParts);
                if ((columns.at(COUNTY_COLUMN) == COUNTY_NAME)
                    && (columns.at(E5_COLUMN).length() != 0))
                {
                    displayValue    dv;
                    float           keyV;

                    dv.city = columns.at(CITY_COLUMN);
                    dv.county = columns.at(COUNTY_COLUMN);
                    dv.postCode = columns.at(POST_CODE_COLUMN);
                    dv.e5 = columns.at(E5_COLUMN);
                    dv.e10 = columns.at(E10_COLUMN);
                    dv.b7p = columns.at(B7P_COLUMN);
                    dv.b7s = columns.at(B7S_COLUMN);

                    setMaxSize(&(dvs.maxCity), dv.city.length());
                    setMaxSize(&(dvs.maxCounty), dv.county.length());
                    setMaxSize(&(dvs.maxPostCode), dv.postCode.length());
                    setMaxSize(&(dvs.maxE5), dv.e5.length());
                    setMaxSize(&(dvs.maxE10), dv.e10.length());
                    setMaxSize(&(dvs.maxB7p), dv.b7p.length());
                    setMaxSize(&(dvs.maxB7s), dv.b7s.length());
                    keyV = dv.e5.toFloat();

                    dvs.values.insert(keyV, dv);
                }
            }

            // Now display the values
            QMultiMapIterator<float, struct displayValue>   iDp(dvs.values);

            // Note adding in setFieldWidth to zero before outputting spaces as
            // the spaces gets a field width as previously set
            while(iDp.hasNext())
            {
                iDp.next();
                ts.setFieldAlignment(QTextStream::AlignLeft);
                ts.setFieldWidth(dvs.maxCity);
                ts << iDp.value().city;
                ts.setFieldWidth(0);
                ts << " ";
                ts.setFieldWidth(dvs.maxCounty);
                ts << iDp.value().county;
                ts.setFieldWidth(0);
                ts << " ";
                ts.setFieldWidth(dvs.maxPostCode);
                ts << iDp.value().postCode;
                ts.setFieldWidth(0);
                ts << " ";
                ts.setFieldWidth(dvs.maxE5);
                ts << iDp.value().e5;
                ts.setFieldWidth(0);
                ts << " ";
                ts.setFieldWidth(dvs.maxE10);
                ts << iDp.value().e10;
                ts.setFieldWidth(0);
                ts << " ";
                ts.setFieldWidth(dvs.maxB7p);
                ts << iDp.value().b7p;
                ts.setFieldWidth(0);
                ts << " ";
                ts.setFieldWidth(dvs.maxB7s);
                ts << iDp.value().b7s;
                ts.setFieldWidth(0);
                ts << Qt::endl;
                ts.flush();
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
