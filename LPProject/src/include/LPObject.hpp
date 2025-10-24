
/**
 * LPObject is the bas Class for LPProject
 * @param a the first
 * @pram b the second
 * @result the reslut
*/  

enum Types{Briefing, Document, eForm };
class LPObject
{
  public:
    LPObject();
    ~LPObject();
    Types Type;
    float Xpos, Ypos;
};
