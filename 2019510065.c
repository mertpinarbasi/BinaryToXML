#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct record
{
  char name[64];    //utf16
  char surname[32]; //utf8
  char gender;
  char email[32];
  char phone_number[16];
  char address[32];
  char level_of_education[8];
  unsigned int income_level; // given little-endian
  unsigned int expenditure;  // given big-endian
  char currency_unit[16];
  char currentMood[32];
  float height;
  unsigned int weight;
};

struct tag
{
  char nameTag[64];
  char surnameTag[32];
  char genderTag[16];
  char emailTag[32];
  char phone_numberTag[16];
  char addressTag[32];
  char level_of_educationTag[32];
  char income_levelTag[16]; // given little-endian
  char expenditureTag[16];
  char currency_unitTag[16];
  char heightTag[16];
  char currentMoodTag[32];
  char weightTag[16];
};

int swap_Endians(int value)
{

  // This var holds the leftmost 8
  // bits of the output.

  int leftmost_byte;

  // This holds the left middle
  // 8 bits of the output

  int left_middle_byle;

  // This holds the right middle
  // 8 bits of the output

  int right_middle_byte;

  // This holds the rightmost
  // 8 bits of the output

  int rightmost_byte;

  // To store the result
  // after conversion

  int result;

  // Get the rightmost 8 bits of the number
  // by anding it 0x000000FF. since the last
  // 8 bits are all ones, the result will be the
  // rightmost 8 bits of the number. this will
  // be converted into the leftmost 8 bits for the
  // output (swapping)

  leftmost_byte = (value & 0x000000FF) >> 0;

  // Similarly, get the right middle and left
  // middle 8 bits which will become
  // the left_middle bits in the output

  left_middle_byle = (value & 0x0000FF00) >> 8;

  right_middle_byte = (value & 0x00FF0000) >> 16;

  // Get the leftmost 8 bits which will be the
  // rightmost 8 bits of the output

  rightmost_byte = (value & 0xFF000000) >> 24;

  // Left shift the 8 bits by 24
  // so that it is shifted to the
  // leftmost end

  leftmost_byte <<= 24;

  // Similarly, left shift by 16
  // so that it is in the left_middle
  // position. i.e, it starts at the
  // 9th bit from the left and ends at the
  // 16th bit from the left

  left_middle_byle <<= 16;

  right_middle_byte <<= 8;

  // The rightmost bit stays as it is
  // as it is in the correct position

  rightmost_byte <<= 0;

  // Result is the concatenation of all these values.

  result = (leftmost_byte | left_middle_byle | right_middle_byte | rightmost_byte);

  return result;
}

int main(int argc, char *argv[])
{
  FILE *fp;
  char line[252];
  struct record person;
  struct tag tag;
  fp = fopen(argv[1], "rb");
  if (fp == NULL)
  {
    printf("Error opening file!");
    exit(1);
  }

  char rootTag[16];
  strcpy(rootTag, argv[2]);

  FILE *xmlwrite = fopen(strcat(argv[2], ".xml"), "w");

  int i = 0;
  while (!feof(fp) && i != 51)
  {

    fread(&person, sizeof(struct record), 1, fp);
    if (i == 0)
    {

      strcpy(tag.nameTag, person.name);
      strcpy(tag.surnameTag, person.surname);

      sprintf(tag.genderTag, "%c", person.gender);
      if (strcmp(tag.genderTag, "gender") != 0)
      {
        strcpy(tag.genderTag, "gender");
      }
      strcpy(tag.emailTag, person.email);
      strcpy(tag.phone_numberTag, person.phone_number);
      strcpy(tag.addressTag, person.address);

      strcpy(tag.level_of_educationTag, person.level_of_education);
      if (strcmp(tag.level_of_educationTag, "level_of_education") != 0)
      {
        strcpy(tag.level_of_educationTag, "level_of_education");
      }

      sprintf(tag.income_levelTag, "%u", person.income_level);
      if (strcmp(tag.income_levelTag, "income_level") != 0)
      {
        strcpy(tag.income_levelTag, "income_level");
      }
      sprintf(tag.expenditureTag, "%u", person.expenditure);
      if (strcmp(tag.expenditureTag, "expenditure") != 0)
      {
        strcpy(tag.expenditureTag, "expenditure");
      }
      strcpy(tag.currency_unitTag, person.currency_unit);
      strcpy(tag.currentMoodTag, person.currentMood);

      sprintf(tag.heightTag, "%u", person.height);

      if (strcmp(tag.heightTag, "height") != 0)
      {
        strcpy(tag.heightTag, "height");
      }

      sprintf(tag.weightTag, "%u", person.weight);
      if (strcmp(tag.weightTag, "weight") != 0)
      {
        strcpy(tag.weightTag, "weight");
      }

      fprintf(xmlwrite, "<%s> \n", rootTag);
    }
    else
    {
      int expenditureSwapped = 0;
      int income_levelSwapped = 0;
      expenditureSwapped = swap_Endians(person.expenditure);
      income_levelSwapped = swap_Endians(person.income_level);

      fprintf(xmlwrite, "    <row id=\"%d\">\n", i);
      fprintf(xmlwrite, "        <%s>%s</%s> \n", tag.nameTag, person.name, tag.nameTag);
      fprintf(xmlwrite, "        <%s>%s</%s> \n", tag.surnameTag, person.surname, tag.surnameTag);
      fprintf(xmlwrite, "        <%s>%c</%s> \n", tag.genderTag, person.gender, tag.genderTag);
      fprintf(xmlwrite, "        <%s>%s</%s> \n", tag.emailTag, person.email, tag.emailTag);
      fprintf(xmlwrite, "        <%s>%s</%s> \n", tag.phone_numberTag, person.phone_number, tag.phone_numberTag);
      fprintf(xmlwrite, "        <%s>%s</%s> \n", tag.addressTag, person.address, tag.addressTag);
      fprintf(xmlwrite, "        <%s>%s</%s> \n", tag.level_of_educationTag, person.level_of_education, tag.level_of_educationTag);
      fprintf(xmlwrite, "        <%s bigEnd=\"%u\">%u</%s> \n", tag.income_levelTag, income_levelSwapped, person.income_level, tag.income_levelTag);
      fprintf(xmlwrite, "        <%s bigEnd=\"%u\" >%u</%s> \n", tag.expenditureTag, person.expenditure, expenditureSwapped, tag.expenditureTag);
      fprintf(xmlwrite, "        <%s>%s</%s> \n", tag.currency_unitTag, person.currency_unit, tag.currency_unitTag);
      fprintf(xmlwrite, "        <%s>%s</%s> \n", tag.currentMoodTag, person.currentMood, tag.currentMoodTag);
      fprintf(xmlwrite, "        <%s>%.2f</%s> \n", tag.heightTag, person.height, tag.heightTag);
      fprintf(xmlwrite, "        <%s>%u</%s> \n", tag.weightTag, person.weight, tag.weightTag);
      fprintf(xmlwrite, "    </row>");
      fprintf(xmlwrite, " \n");
    }

    i++;
  }

  fprintf(xmlwrite, "</%s> \n", rootTag);
  fclose(fp);
  fclose(xmlwrite);
  return 0;
}
