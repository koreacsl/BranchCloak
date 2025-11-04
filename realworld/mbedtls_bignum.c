/*
 * Sliding-window exponentiation: X = A^E mod N  (HAC 14.85)
 */
int mbedtls_mpi_exp_mod( mbedtls_mpi *X, const mbedtls_mpi *A,
                         const mbedtls_mpi *E, const mbedtls_mpi *N,
                         mbedtls_mpi *prec_RR )
{
    ...
    
    while( 1 )
    {
        if( bufsize == 0 )
        {
            if( nblimbs == 0 )
                break;

            nblimbs--;

            bufsize = sizeof( mbedtls_mpi_uint ) << 3;
        }

        bufsize--;

        ei = (E->p[nblimbs] >> bufsize) & 1;

        // skip leading 0s
        if( ei == 0 && state == 0 )
            continue;

        if( ei == 0 && state == 1 )
        {
            // out of window, square X
            mpi_montmul( X, X, N, mm, &T );
            continue;
        }

        // add ei to current window
        state = 2;

        nbits++;
        wbits |= ( ei << ( wsize - nbits ) );

        if( nbits == wsize )
        {
            // X = X^wsize R^-1 mod N
            for( i = 0; i < wsize; i++ )
                mpi_montmul( X, X, N, mm, &T );

            // X = X * W[wbits] R^-1 mod N
            MBEDTLS_MPI_CHK( mpi_select( &WW, W, (size_t) 1 << wsize, wbits ) );
            mpi_montmul( X, &WW, N, mm, &T );

            state--;
            nbits = 0;
            wbits = 0;
        }
    }

    /*
     * process the remaining bits
     */
    for( i = 0; i < nbits; i++ )
    {
        mpi_montmul( X, X, N, mm, &T );

        wbits <<= 1;

        if( ( wbits & ( one << wsize ) ) != 0 )
            mpi_montmul( X, &W[1], N, mm, &T );
    }

    /*
     * X = A^E * R * R^-1 mod N = A^E mod N
     */
    mpi_montred( X, N, mm, &T );

    if( neg && E->n != 0 && ( E->p[0] & 1 ) != 0 )
    {
        X->s = -1;
        MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( X, N, X ) );
    }

    //asm volatile("mfence");
    asm volatile("SAFE1_end:"); //SAFE end

cleanup:

    for( i = ( one << ( wsize - 1 ) ); i < ( one << wsize ); i++ )
        mbedtls_mpi_free( &W[i] );

    mbedtls_mpi_free( &W[1] ); mbedtls_mpi_free( &T ); mbedtls_mpi_free( &Apos );
    mbedtls_mpi_free( &WW );

    if( prec_RR == NULL || prec_RR->p == NULL )
        mbedtls_mpi_free( &RR );

    return( ret );
}