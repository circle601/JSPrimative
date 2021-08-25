using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FSMTree.BNF
{
    class BNFExpression
    {
    }

    class LiteralExpression : BNFExpression
    {
    }

    class IdentExpression : BNFExpression
    {
    }

    class ListExpression : BNFExpression
    {
        public ListExpression (IEnumerable<BNFExpression> parts)
        {
            this.Parts = parts.ToArray();
        }
        BNFExpression[] Parts;
    }

}
