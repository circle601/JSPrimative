using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace FSMTree.BNF
{
    class Lexer
    {

        StreamReader Input;

        struct BNFToken
        {
            int Pos;
            int Line;
            object Data;
        }


        public char Peak()
        {
            if (Input.EndOfStream) throw new Exception("End of file");
            return (char)Input.Peek();
        }






   

        public string LexString(char Ending)
        {
            StringBuilder sb = new StringBuilder();
            char ch;
            while ((ch = Peak()) != Ending)
            {
                sb.Append(ch);
            }
            return sb.ToString();
        }

        
    }
}
