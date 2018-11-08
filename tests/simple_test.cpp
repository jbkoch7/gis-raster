
// --- App Includes --- //
#include <gis/Raster.h>

// --- Standard Includes --- //
#include <iostream>
#include <string>

////////////////////////////////////////////////////////////////////////////////
int main(
    int argc,
    char** argv)
{
    try
    {
        gis::Raster< float, float, int > raster;
        gis::Band< float >& a = raster.band< 0 >();
        a.scale( 0.0 );
        a = std::move( gis::Band< float >( 10, 10, 2.0, 7.0 ) );
        std::cout << std::endl << "rast1:" << std::endl;
        std::cout << a.width() << std::endl;
        std::cout << a( 0, 1 ) << std::endl;

        gis::Raster< float, int > rast2(
            gis::Band< float >( 20, 20, 3.0, 6.765 ),
            gis::Band< int >( 20, 20, 40.501, 60.666 ) );
        std::cout << std::endl << "rast2:" << std::endl;
        std::cout << rast2.band< 0 >().width() << std::endl;
        std::cout << rast2.band< 0 >()( 0, 10 ) << std::endl;
        std::cout << rast2.band< 1 >().nodatavalue() << std::endl;
        std::cout << rast2.band< 1 >()( 0, 10 ) << std::endl;
        std::cout << rast2.band< 1 >()[ 0 ][ 10 ] << std::endl;

        gis::Raster< int, float, double > rast3(
            20, 20,
            6, 7.2f, 5.5,
            7, 8.3f, 6.6 );
        std::cout << std::endl << "rast3:" << std::endl;
        std::cout << rast3.band< 0 >().nodatavalue() << std::endl;
        std::cout << rast3.band< 1 >().nodatavalue() << std::endl;
        std::cout << rast3.band< 2 >().nodatavalue() << std::endl;
        std::cout << rast3.band< 0 >()( 0, 10 ) << std::endl;
        std::cout << rast3.band< 1 >()( 0, 10 ) << std::endl;
        std::cout << rast3.band< 2 >()( 0, 10 ) << std::endl;

        gis::Raster< int, float, double > rast4(
            20, 20,
            { 6, 7.2f, 4.4 },
            { 7, 8.3f, 5.5 } );
        std::cout << std::endl << "rast4:" << std::endl;
        std::cout << rast4.band< 0 >().nodatavalue() << std::endl;
        std::cout << rast4.band< 1 >().nodatavalue() << std::endl;
        std::cout << rast4.band< 2 >().nodatavalue() << std::endl;
        std::cout << rast4.band< 0 >()( 0, 10 ) << std::endl;
        std::cout << rast4.band< 1 >()( 0, 10 ) << std::endl;
        std::cout << rast4.band< 2 >()( 0, 10 ) << std::endl;
        std::cout << rast4.size() << std::endl;
        std::cout << "same size: " << rast4.same_size() << std::endl;

        gis::Raster< int > rast5(
            20, 20,
            std::make_tuple( 6 ),
            std::make_tuple( 7 ) );
        std::cout << std::endl << "rast5:" << std::endl;
        std::cout << rast5.band().nodatavalue() << std::endl;
        std::cout << rast5.band()( 0, 10 ) << std::endl;
        std::cout << rast5.size() << std::endl;
        rast5.band< 0 >()( 0, 0 ) = rast5.band< 0 >().nodatavalue();
        rast5.band< 0 >()( 0, 1 ) = 8;
        rast5.band< 0 >() *= 3.4;
        //rast5.band< 0 >()( 0, 2 ) = rast5.band< 0 >().nodatavalue();
        std::cout << "count: " << rast5.band< 0 >().count() << std::endl;
        std::cout << "min: " << rast5.band< 0 >().min() << std::endl;
        std::cout << "max: " << rast5.band< 0 >().max() << std::endl;
        std::cout << "sum: " << rast5.band< 0 >().sum() << std::endl;
        std::cout << "avg: " << rast5.band< 0 >().avg() << std::endl;
        std::cout << "same size: " << rast5.same_size() << std::endl;

        //rast5.append( gis::Band< double >( 20, 20, 3.0, 6.765 ) );

        gis::Raster< int > rast6;
    }
    catch( std::exception const& e )
    {
        std::cerr << "*** std::exception caught: "
            << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////
