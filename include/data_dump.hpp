#ifndef ENGINE_DATA_DUMP_HPP_
#define ENGINE_DATA_DUMP_HPP_

namespace evl
{
	namespace utility
	{
		static void data_dump( FILE* file, const void* source, int howmuch, const char* title = NULL)
		{
			if (title != NULL)
			{
				if (file != 0)
				{
					fprintf(file, "============== %10s ==============\n", title);
				}
				else
				{
					printf("============== %10s ==============\n", title);
				}
			}

			int line; // output lines 
			int posInSource ; // travels and output howmuch hex 
			char outputOneline[ 256] = {0, }; // one line with 256 chars

			static int mostHexes = 16 ; // one line contains 16 hexes
			char part3[ 16 + 1] = { 0, }; // part3 will output 16 hexes plus one termal char 
			int hexIndex; // travels all the 16 hexes
			unsigned int hexValue; // thread the minumute hex as a char 

			char hexNode[ 4] = {0, } ; // a int contains 4 bytes = 4 * 8 bits = 32 bits = 32 bits / 4 = 8 hexes 
			// even thouth char hexNode[1] will be ok to have 2 hexes , 
			// but for unsigned int hexValue is too large 

			posInSource = 0;

			for( line = 0; howmuch > 0 ; ++ line )
			{
				// start a new line 
				memset( outputOneline, 0 , sizeof( outputOneline) );
				memset( part3, 0, sizeof( part3) );

				// output part1 : line number.
				sprintf( outputOneline, "%08X : ", line);


				// generate 16 hexes 
				hexIndex = 0;
				for( ; hexIndex < mostHexes; ++ hexIndex, -- howmuch)
				{
					if( howmuch < 1) 
						break;

					// thread the minumute hex as a char 
					hexValue = *(unsigned char*)((unsigned char*)source + posInSource);
					++ posInSource;

					// part2:
					memset( hexNode, 0, sizeof( hexNode) );
					if( hexIndex == 7)
					{
						sprintf( hexNode, "%02X\t", hexValue);
					}else{
						sprintf( hexNode, "%02X ", hexValue);
					}

					strcat( outputOneline, hexNode ); // about 06X hex in hexNode will be discarded automaticly as the memset() functon

					// part3:
					if( hexValue >= '!' && hexValue <= '~' )
					{
						hexNode[ 0] = hexValue;
					}else{
						hexNode[ 0] = '.';
					}
					hexNode[1] = '\0';
					strcat( part3, hexNode );

				}

				// fill unneeded hexes 
				if( hexIndex > 0 && hexIndex < mostHexes - 1 )
				{
					for( ; hexIndex < mostHexes ; ++ hexIndex)
					{
						strcat( outputOneline, "  " );
					}
				}

				// concat with part3 
				strcat( outputOneline, " ; " );
				strcat( outputOneline, part3 );

				// output line generated
				if( file != 0 )
				{
					fprintf( file, "%s\n", outputOneline );
				}else{
					printf( "%s\n", outputOneline);
				}
			}
		}
	}// namespace utility
} // namespace evl

#endif // ENGINE_DATA_DUMP_HPP_