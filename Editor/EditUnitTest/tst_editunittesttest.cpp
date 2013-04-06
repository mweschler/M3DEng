#include <QString>
#include <QtTest>
#include <QDebug>
#include <QVector>
#include <QVector3D>

#include "../M3DEdit/box.h"
#include "../M3DEdit/geometrymanager.h"

class EditUnitTestTest : public QObject
{
    Q_OBJECT
    
public:
    EditUnitTestTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void boxTestDefault();
    void geoMgrTest();
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

void EditUnitTestTest::geoMgrTest(){
    M3DEditLevel::GeometryManager mgr;
    QVERIFY(mgr.total() == 0);

    M3DEditLevel::Box box;
    int id = mgr.addGeometry(&box);
    QVERIFY(id == 0);
    QVERIFY(mgr.total() == 1);

    M3DEditLevel::Box rv = *(M3DEditLevel::Box *)mgr.getGeometry(id);
    QVector<QVector3D> verts = rv.getVerticies();
    QVERIFY(verts.size() == 8);

    M3DEditLevel::Box box2, box3;
    int id2 = mgr.addGeometry(&box2);
    QVERIFY(mgr.total()== 2);
    QVERIFY(id2 == 1);

    mgr.removeGeometry(id);
    QVERIFY(mgr.total() == 1);

    int id3 = mgr.addGeometry(&box3);
    QVERIFY(mgr.total() == 2);
    QVERIFY(id3 == 0);

    mgr.removeGeometry(id2);
    mgr.removeGeometry(id3);

    for(int i = 0; i< 10000; ++i)
    {
        M3DEditLevel::Box *temp = new M3DEditLevel::Box();
        mgr.addGeometry(temp);
    }
    QVERIFY(mgr.total() == 10000);

    for(int i = 0; i< 10000; ++i){
        M3DEditLevel::Box *temp = (M3DEditLevel::Box *)mgr.removeGeometry(i);
        delete temp;
    }
    QVERIFY(mgr.total() == 0);
}

/*void EditUnitTestTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}*/

QTEST_APPLESS_MAIN(EditUnitTestTest)

#include "tst_editunittesttest.moc"
