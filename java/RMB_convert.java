//人民币大小写换算程序


public class ParseRMB{
    static String number="零壹贰叁肆伍陆柒捌玖";
    static String unit="分角元拾佰仟万拾佰仟亿拾佰仟";
    public static void main(String[]args){
        System.out.println(parse((args[0])));
    }
    public static String parse(String money){
        int point_pos=money.indexOf(".");
        String interger_part="",decimal_part="",is_interger="";
        if (point_pos==-1)
        {
            is_interger="整";
            interger_part=money.substring(0);
        }
        else
        {
            interger_part=money.substring(0,point_pos);
            decimal_part=money.substring(point_pos+1);
        }

        String result="人民币";
        result+=(parseInterger(interger_part)+is_interger+parseDecimal(decimal_part));
        return result;
    }
    public static String parseInterger(String money){
        String front_part="",back_part="";
        int len=money.length();
        int unit_pos=len+1;
        boolean is_zero=false,is_m=false,is_b=false;

        for (int i=0;i<len;++i)
        {
            char item=money.charAt(i);
            if (item=='0')
            {
                if (unit_pos==2)
                    back_part+=unit.charAt(unit_pos);
                else if (unit_pos==10&&is_b)
                    back_part+=unit.charAt(10);
                else if (unit_pos==6&&is_m)
                    back_part+=unit.charAt(6);
                is_zero=true;
                --unit_pos;
                continue;
            }
            if (is_zero)
            {
                if (unit_pos!=5&&unit_pos!=9)
                    back_part+=number.charAt(0);
                is_zero=false;
            }
            if (unit_pos>=10)
                is_b=true;
            else if (unit_pos>=6)
                is_m=true;
            back_part+=number.charAt(item-'0');
            back_part+=unit.charAt(unit_pos--);
        }
        if (len<money.length())
            back_part+=unit.charAt(2);
        return front_part+back_part;
    }
    public static String parseDecimal(String money){
        String result="";
        for (int i=0;i<money.length();++i)
        {
            char item=money.charAt(i);
            if (item=='0')
                continue;
            result=result+number.charAt(item-'0')+unit.charAt(1-i);
        }
        return result;

    }

}
