/**
 * @file TestConnection.cpp
 * @author Nitish Maindoliya, Dominik Leisinger, Bram Hogg
 */
 
#include "TestConnection.h"

void TestConnection::test_constructors() {
    //default constructor
    Connection defaultConn;
    QCOMPARE(defaultConn.get_source(), nullptr);
    QCOMPARE(defaultConn.get_target(), nullptr);
    QCOMPARE(defaultConn.get_simulation(), nullptr);
    QCOMPARE(defaultConn.get_name(), QString("Test"));
    QCOMPARE(defaultConn.get_rate_expression(), QString("0"));

    //variable constructor
    Compartment sourcecomp;
    Compartment targetcomp;
    QString name = "TestName";
    QString rateexp = "0.01";

    Connection paramconn(name, &sourcecomp, &targetcomp, rateexp);
    QCOMPARE(paramconn.get_source(), &sourcecomp);
    QCOMPARE(paramconn.get_target(), &targetcomp);
    QCOMPARE(paramconn.get_name(), name);
    QCOMPARE(paramconn.get_rate_expression(), rateexp);
}


void TestConnection::test_getter_setters() {

    Connection defaultconn;

    //Name Section
    QCOMPARE(defaultconn.get_name(), QString("Test"));

    defaultconn.set_name("Test Name");
    QCOMPARE(defaultconn.get_name(), QString("Test Name"));


    // Name with invalid characters - no change
    defaultconn.set_name("InvalidName!!");
    QCOMPARE(defaultconn.get_name(), QString("Test Name"));


    // Empty name - no change
    defaultconn.set_name("");
    QCOMPARE(defaultconn.get_name(), QString("Test Name"));

    //more name set/get
    Connection connection;

    connection.set_name("tested");
    QCOMPARE(connection.get_name(), QString("tested"));


    // Empty name - no change
    connection.set_name("");
    QCOMPARE(connection.get_name(), QString("tested"));

    //Source/Target Section

    Compartment SourceComp;
    Compartment TargetComp;

    QCOMPARE(defaultconn.get_source(), nullptr);
    QCOMPARE(defaultconn.get_target(), nullptr);

    defaultconn.set_source(&SourceComp);
    defaultconn.set_target(&TargetComp);

    QCOMPARE(defaultconn.get_source(), &SourceComp);
    QCOMPARE(defaultconn.get_target(), &TargetComp);

    defaultconn.set_target(&SourceComp);
    QCOMPARE(defaultconn.get_target(), &SourceComp);

    // more set/get targets and sources
    Compartment* compartment1;
    Compartment* compartment2;

    connection.set_source(compartment1);
    connection.set_target(compartment2);
    QCOMPARE(connection.get_source(), compartment1);
    QCOMPARE(connection.get_target(), compartment2);

    Compartment* compartment3;
    connection.set_source(compartment3);
    connection.set_target(compartment1);
    QCOMPARE(connection.get_source(), compartment3);
    QCOMPARE(connection.get_target(), compartment1);

    //Rate expression section
    QCOMPARE(defaultconn.get_rate_expression(), "0");
    defaultconn.set_rate_expression("0.001");
    QCOMPARE(defaultconn.get_rate_expression(), "0.001");

    QString rate_exp = "HHHlskk";
    QString rate_exp2 = "(09..,/sdf/2";

    connection.set_rate_expression(rate_exp);
    QCOMPARE(connection.get_rate_expression(), "HHHlskk");
    connection.set_rate_expression(rate_exp2);
    QCOMPARE(connection.get_rate_expression(), "(09..,/sdf/2");

    rate_exp = "LJKMNB99s.";
    connection.set_rate_expression(rate_exp);
    QCOMPARE(connection.get_rate_expression(), "LJKMNB99s.");

    QString rate_exp3 = " ";
    connection.set_rate_expression(rate_exp3);
    QCOMPARE(connection.get_rate_expression(), " ");

    //Simulation Section
//    QCOMPARE(defaultconn.get_simulation(), nullptr);
//    Simulation sim;
//    defaultconn.set_simulation(&sim);
//    QCOMPARE(defaultconn.get_simulation(), &sim);
//
//    // more set/get simulation
//    Simulation* simulation1;
//    Simulation* simulation2;
//    connection.set_simulation(simulation1);
//    QCOMPARE(connection.get_simulation(), simulation1);
//
//    connection.set_simulation(simulation2);
//    QCOMPARE(connection.get_simulation(), simulation2);

}
void TestConnection::test_add_connection() {}
void TestConnection::test_update_connection() {}

#include "TestConnection.moc"
