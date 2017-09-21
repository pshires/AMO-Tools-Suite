#include "catch.hpp"
#include <calculator/motor/MotorEfficiency.h>
#include <calculator/motor/MotorEfficiency25.h>

TEST_CASE( "Calculate Motor Efficiency - 200 HP", "[MotorEfficiency]" ) {
    MotorEfficiency motorEff(Motor::LineFrequency::FREQ60, 1785, Motor::EfficiencyClass::ENERGY_EFFICIENT, 0, 200, 0);

    {
        motorEff.setLoadFactor( 0.0 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.0 ) );
    }

    {
        motorEff.setLoadFactor( 0.25 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.930393384 ) );
    }

    {
        motorEff.setLoadFactor( 0.5 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.949504131 ) );
    }

    {
        motorEff.setLoadFactor( 0.75 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.9569034 ) );
    }

    {
        motorEff.setLoadFactor( 1.0 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.956259245 ) );
    }

    {
        motorEff.setLoadFactor( 1.25 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.946696653 ) );
    }

}

TEST_CASE( "Calculate Motor Efficiency - 100 HP", "[MotorEfficiency]" ) {
    MotorEfficiency motorEff(Motor::LineFrequency::FREQ60, 1785, Motor::EfficiencyClass::ENERGY_EFFICIENT, 0, 100, 0);

    {
        motorEff.setLoadFactor( 0.0 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.0 ) );
    }

    {
        motorEff.setLoadFactor( 0.25 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.921519665 ) );
    }

    {
        motorEff.setLoadFactor( 0.5 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.946047805 ) );
    }

    {
        motorEff.setLoadFactor( 0.75 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.951033931 ) );
    }

    {
        motorEff.setLoadFactor( 1.0 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.949413679 ) );
    }

    {
        motorEff.setLoadFactor( 1.25 );
        INFO( "Load Factor == " << motorEff.getLoadFactor() );
        CHECK( motorEff.calculate() == Approx( 0.939919542 ) );
    }
}

TEST_CASE( "MotorEfficiency25 ", "[MotorEfficiency]" ) {
    MotorEfficiency25 me25(Motor::EfficiencyClass::PREMIUM, 200, 1780, Motor::LineFrequency::FREQ60);
    auto results = me25.calculate();
    CHECK(results[0] == Approx(0.9359788571));
    CHECK(results[1] == Approx(0.9552043325));
    CHECK(results[2] == Approx(0.9626480215));
    CHECK(results[3] == Approx(0.962));
    CHECK(results[4] == Approx(0.95238));
}
