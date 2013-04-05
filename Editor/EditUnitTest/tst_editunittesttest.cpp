#include <QString>
#include <QtTest>
#include <QDebug>
#include <QVector>
#include <QVector3D>

#include "../M3DEdit/box.h"

class EditUnitTestTest : public QObject
{
    Q_OBJECT
    
public:
    EditUnitTestTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void boxTestDefault();
    //void testCase1_data();
};

EditUnitTestTest::EditUnitTestTest()
{
}

void EditUnitTestTest::initTestCase()
{
}

void EditUnitTestTest::cleanupTestCase()
{
}

void EditUnitTestTest::boxTestDefault()
{
    M3DEditLevel::Box box;
    QVector<QVector3D> verts = box.getVerticies();
    QVERIFY(verts.size() == 8);
    for(int i = 0; i < 8; ++i){
        QVector3D vert = verts[i];
        qDebug()<< "Vert ["<<i<<"]: "<<vert;
    }

    for(int i = 0; i <2; ++i){
        for(int j = 0; j< 2;  ++j)
        {
            for(int k = 0; k <2; ++k)
            {
                int index = (i * 4) + (j * 2) + k;
                QVector3D vert = verts[index];
                QVERIFY(vert.x() == i && vert.y() == j && vert.z() == k);
            }
        }
    }

}

/*void EditUnitTestTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}*/

QTEST_APPLESS_MAIN(EditUnitTestTest)

#include "tst_editunittesttest.moc"
