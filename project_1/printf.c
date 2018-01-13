

#include <stdio.h>


void show_mode(void)
{
    printf("\033[2J");
    printf("\033[47;30m\033[2;15H┌───────────────────────────────────────────────────┐\033[0m\n");
    printf("\033[47;30m\033[3;15H│                  学生信息管理系统                 │\033[0m\n");
    printf("\033[47;30m\033[4;15H├───────────────────────────────────────────────────┤\033[0m\n");
    printf("\033[47;30m\033[5;15H│                      功能选择                     │\033[0m\n");
    printf("\033[47;30m\033[6;15H├───────────────────────────────────────────────────┤\033[0m\n");
    printf("\033[47;30m\033[7;15H│                  输入1录入学生信息                │\033[0m\n");
    printf("\033[47;30m\033[8;15H│                  输入2查询学生信息                │\033[0m\n");
    printf("\033[47;30m\033[9;15H│                  输入3插入学生信息                │\033[0m\n");
    printf("\033[47;30m\033[10;15H│                  输入4删除学生信息                │\033[0m\n");
    printf("\033[47;30m\033[11;15H│                  输入5修改学生信息                │\033[0m\n");
    printf("\033[47;30m\033[12;15H│                  输入6排名学生成绩                │\033[0m\n");
    printf("\033[47;30m\033[13;15H│                  输入7统计学生人数                │\033[0m\n");
    printf("\033[47;30m\033[14;15H│                  输入8统计挂科情况                │\033[0m\n");
    printf("\033[47;30m\033[15;15H│                  输入9显示学生信息                │\033[0m\n");
    printf("\033[47;30m\033[16;15H│                  输入10退出管理系统               │\033[0m\n");
    printf("\033[47;30m\033[17;15H└───────────────────────────────────────────────────┘\033[0m\n");
}

int choice_mode(void)
{
    char i,c,flag=0,num=1;
    while(1)
    {
        printf("\033[?25l");
        system("stty -echo");
        system("stty -icanon");
        scanf("%c", &i);        
        if(i == '8')
        {
            if(num == 1)
                num = 10;
            else
                num--;
        }
        else if(i == '2')
        {
            if(num == 10)
                num=1;
            else
                num++;
        }
        else if(i == '5')
            return num; 
        
        switch(num)
        {
            case 1:
            {
                show_mode();
                printf("\033[47;34m\033[7;15H│                  输入1录入学生信息                │\033[0m\n");
                break;
            }
            case 2:
            {
                show_mode();
                printf("\033[47;34m\033[8;15H│                  输入2查询学生信息                │\033[0m\n");
                break;
            }
            case 3:
            {
                show_mode();
                printf("\033[47;34m\033[9;15H│                  输入3插入学生信息                │\033[0m\n");
                break;
            }
            case 4:
            {
                show_mode();
                printf("\033[47;34m\033[10;15H│                  输入4删除学生信息                │\033[0m\n");
                break;
            }
            case 5:
            {
                show_mode();
                printf("\033[47;34m\033[11;15H│                  输入5修改学生信息                │\033[0m\n");
                break;
            }
            case 6:
            {
                show_mode();
                printf("\033[47;34m\033[12;15H│                  输入6排名学生成绩                │\033[0m\n");
                break;
            }
            case 7:
            {
                show_mode();
                printf("\033[47;34m\033[13;15H│                  输入7统计学生人数                │\033[0m\n");
                break;
            }
            case 8:
            {
                show_mode();
                printf("\033[47;34m\033[14;15H│                  输入8统计挂科情况                │\033[0m\n");
                break;
            }
            case 9:
            {
                show_mode();
                printf("\033[47;34m\033[15;15H│                  输入9显示学生信息                │\033[0m\n");
                break;
            }
            case 10:
            {
                show_mode();
                printf("\033[47;34m\033[16;15H│                  输入10退出管理系统               │\033[0m\n");
                break;
            }
        }
    }
}

int main(void)
{   
    int i;
    show_mode();
    i = choice_mode();
    
    printf("\n");
    printf("\n");
    printf("\n");
    system("stty echo");
    system("stty icanon");
    printf("\033[?25h");
    return i;
}