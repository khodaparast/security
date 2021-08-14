



 for j in 1000 2000 4000 8000 16000 32000 64000 128000  256000 512000 1000000 2000000 3000000 4000000;
    do
        for i in 1 2 3 4 5 6 7 8 9 10;
        do
            #  echo "AES encryption/decryption result for:  $j, $i \n" >>  AES_ENC_DEC_bench.txt
            ./AESENC test.txt $j >> AES_ENC_bench.txt
            
            # or do whatever with individual element of the array
        done
        # echo "-------------------------------------------------------------------------- \n" >> AES_ENC_DEC_bench.txt
    done