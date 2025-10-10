#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    int pin;
    int blocked;
    char login[7]
} struct_user;

struct_user* users = NULL;
int capacity = 16;
int count = 0;

int users_camparer(const void* a, const void* b)
{
    return strcmp(((struct_user*)a)->login, ((struct_user*)b)->login);
}

struct_user* find_user(const char* login)
{
    int cmep, miad;
    int left = 0, right = count - 1;
    while (left <= right)
    {
        miad = (left + right) / 2;
        cmep = strcmp(login, users[miad].login);
        if (cmep == 0)
            return &users[miad];
        else if (cmep < 0)
            right = miad - 1;
        else
            left = miad + 1;
    }
    return NULL;
}

int register_user()
{
    char login[7];
    int pin, len;
    struct_user* tempil;

    while (1)
    {
        printf("Enter new login: ");
        scanf("%6s", login);

        len = strlen(login);
        if (len == 0 || len > 6)
        {
            printf("Incorrect (login).\n");
            continue;
        }
        if (find_user(login))
        {
            printf("User exists.\n");
            continue;
        }
        break;
    }

    while (1)
    {
        printf("PIN: ");
        if (scanf("%d", &pin) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid input. Enter a number.\n");
            continue;
        }
        if (pin < 0 || pin > 999999)
        {
            printf("PIN out of range.\n");
            continue;
        }
        break;
    }

    if (count >= capacity || count == 0)
    {
        capacity *= 2;
        tempil = realloc(users, sizeof(struct_user) * capacity);
        if (tempil == NULL)
        {
            return 1;
        }
        users = tempil;
    }

    strcpy(users[count].login, login);
    users[count].pin = pin;
    users[count].blocked = 0;
    count++;

    qsort(users, count, sizeof(struct_user), users_camparer);

    printf("Successfully!\n");
    return 0;
}

struct_user* login_user()
{
    char login[7];
    int pin;

    printf("Enter login: ");

    scanf("%6s", login);

    struct_user* user = find_user(login);
    if (!user)
    {
        printf("User not found =(\n");
        return NULL;
    }
    if (user->blocked)
    {
        printf("User is blocked =(\n");
        return NULL;
    }
    while (1)
    {
        printf("Enter PIN: ");
        scanf("%d", &pin);
        if (user->pin != pin)
        {
            printf("Incorrect PIN.\n");
            continue;
        }
        break;
    }
    printf("Successfully!\n");
    return user;
}

void print_time()
{
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    printf("%02d:%02d:%02d\n",
        tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
}

void print_date()
{
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    printf("%02d:%02d:%04d\n",
        tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
}

void How_much(char* date_str, char* time_str, char* flag)
{
    time_t t1, t2;
    double diff;
    struct tm tm_info;
    memset(&tm_info, 0, sizeof(tm_info));
    tm_info.tm_isdst = -1;
    if (sscanf(date_str, "%d:%d:%d",
        &tm_info.tm_mday, &tm_info.tm_mon, &tm_info.tm_year) != 3)
    {
        printf("Invalid date format\n");
        return;
    }
    if (sscanf(time_str, "%d:%d:%d",
        &tm_info.tm_hour, &tm_info.tm_min, &tm_info.tm_sec) != 3)
    {
        printf("Invalid time format\n");
        return;
    }
    tm_info.tm_mon -= 1;
    tm_info.tm_year -= 1900;

    t1 = mktime(&tm_info);
    if (t1 == (time_t)-1)
    {
        printf("Invalid datetime\n");
        return;
    }
    t2 = time(NULL);
    diff = difftime(t2, t1);
    if (flag == NULL || flag[0] != '-' || flag[1] == '\0')
    {
        printf("Invalid flag\n");
        return;
    }
    if (flag[1] == 's')
        printf("%.0f seconds\n", diff);
    else if (flag[1] == 'm')
        printf("%.0f minutes\n", diff / 60.0);
    else if (flag[1] == 'h')
        printf("%.0f hours\n", diff / 3600.0);
    else if (flag[1] == 'y')
        printf("%.2f years\n", diff / (365.0 * 24.0 * 3600.0));
    else
        printf("Unknown flag\n");
}


void Limitations()
{
    int confirm;
    char username[7];
    printf("Enter username to block: ");
    scanf("%6s", username);
    struct_user* user = find_user(username);
    if (!user)
    {
        printf("User not found.\n");
        return;
    }
    printf("Enter password: ");
    scanf("%d", &confirm);
    if (confirm == 52)
    {
        user->blocked = 1;
        printf("User %s is now blocked.\n", user->login);
    }
    else
        printf("Blocking cancelled.\n");
}

void user_session(struct_user* user)
{
    char command[100];
    char flag[3], data_str[11], time_str[9];
    int log = 0;
    while (1)
    {
        printf("\nEnter command: ");
        scanf("%8s", command);

        switch (command[0])
        {
        case 'T':
            print_time();
            break;
        case 'D':
            print_date();
            break;
        case 'H':
            scanf("%8s %10s %8s %2s", command, data_str, time_str, flag);
            howmuch(data_str, time_str, flag);
            break;
        case 'L':
            log = 1;
            break;
        case 'S':
            sanctions();
            break;
        default:
            printf("Unknown command.\n");
            break;
        }

        if (log)
            break;
    }
}

int main()
{
    int err = 0;
    while (1)
    {
        printf("\n1 - Register\n - Login\n: ");
        int choice;
        scanf("%d", &choice);
        if (choice == 1)
        {
            err = register_user();
            if (err)
                return err;
        }
        else if (choice == 2)
        {
            struct_user* user = login_user();
            if (user)
                user_session(user);
        }
        else
            printf("Invalid choice (\n");
    }
    free(users);
    return 0;
}