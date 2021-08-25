using FSMTree.BNF;
using System;
using System.Collections.Generic;
using System.ComponentModel.Design;
using System.IO;
using System.Linq;
using System.Text;

namespace FSMTree
{
    class BNFParser
    {

        int LineCount = 1;

        readonly StreamReader Input;

        public enum TokenType
        {
            Indent,
            Constant
        }


        public BNFParser(Stream Code)
        {
            Input = new StreamReader(Code);
        }

 


        public class BNFExpression
        {
        }

        public class BNFToken : BNFExpression
        {
            //public int Pos;
            public int Line;
            public object Data;
            public TokenType ttype;
            public override string ToString()
            {
                if(ttype == TokenType.Indent)
                {
                    return "<" + Data.ToString() + ">";
                }
                else
                {
                    return "\"" + Data.ToString() + "\"";
                }
            }
        }

        public class ListExpression : BNFExpression
        {
            BNFExpression[] Parts;

            public ListExpression(IEnumerable<BNFExpression> parts)
            {
                this.Parts = parts.ToArray<BNFExpression>();
            }
           
            public override string ToString()
            {
                return String.Join(" ", Parts.Select(x => x.ToString()));
            }


        }




        protected BNFToken GenerateToken(object Data, TokenType ttype)
        {
            BNFToken result = new BNFToken();
            result.Data = Data;
            result.Line = LineCount;
            result.ttype = ttype;
            return result;
        }




        public char Consume()
        {
            if (Input.EndOfStream) throw new Exception("End of file");
            int ch;
            while (true)
            {
                ch = Input.Read();
                if (ch == -1) throw new Exception("End of file");
                if (ch == '\n')
                {
                    LineCount++;
                }
                else
                {
                    break;
                }
            }
            return (char)ch;
        }

        public void ConsumeWhiteSpace()
        {
            while (Char.IsWhiteSpace((char)Input.Peek()))
            {
                Consume();
            }
        }


        public char Peek()
        {
            int p1 = Input.Peek();
            if (p1 == -1) throw new Exception("; expected. End of file");
            return (char)p1;
        }




        public bool IsCodeSymbol()
        {
            char peek = Peek();
            if (peek == ';') return false ;
            if (peek == '"' || peek == '<') return true;
            if (char.IsPunctuation(peek) || char.IsSymbol(peek) || char.IsLetterOrDigit(peek)) return true;
            return false;
        }



        public string LexString(char Ending)
        {
            StringBuilder sb = new StringBuilder();
            char ch;
            while ((ch = Consume()) != Ending)
            {
                sb.Append(ch);
            }
            return sb.ToString();
        }


        protected BNFToken LexString()
        {
            ConsumeWhiteSpace();
            char StartChar = Consume();
            if (StartChar != '"' && StartChar != '\'') throw new Exception("Indentifier expected");
            return GenerateToken(LexString(StartChar),TokenType.Constant);

        }

        protected BNFToken LexIdent()
        {
            ConsumeWhiteSpace();
            char StartChar = Peek();
            if (char.IsLetter(StartChar)){
                return GenerateToken(LexString(' '), TokenType.Indent);
            }else {
                if (Consume() != '<') throw new Exception("Indentifier expected");
                return GenerateToken(LexString('>'), TokenType.Indent);
            }
          
          

        }

        public char LexSymbol()
        {
            ConsumeWhiteSpace();
            return (char)Consume();
        }

        public void LexPattern(string pattern)
        {
            ConsumeWhiteSpace();
            foreach (var item in pattern)
            {
                if (Consume() != item) throw new Exception(item + " expected");
            }
        }


        public void ParseLine()
        {
            BNFToken Ident = LexIdent();
            LexPattern("::=");
            BNFExpression Expression = ParseExpression();
        }

        public BNFExpression ParseExpression() { 
            ConsumeWhiteSpace();
            List<BNFExpression> parts = new List<BNFExpression>();
            while(IsCodeSymbol()){
                parts.Add(ParseExpressionPart());
            }
            if (Consume() != ';') throw new Exception("; expected");
            return new ListExpression(parts);
        }


        public BNFExpression ParseExpressionPart()
        {
            BNFExpression Result = null;
            ConsumeWhiteSpace();
            char ch = Peek();
            switch (ch)
            {
                case ('<'):
                    Result =  LexIdent();
                    break;
                case ('"'):
                case ('\''):
                    Result = LexString();
                    break;
                default:
                    if (char.IsLetterOrDigit(ch))
                    {
                        Result = LexIdent();
                    }
                    else
                    {

                    }
                    break;
            }
            ConsumeWhiteSpace();
            return Result;
        }



    }
}
