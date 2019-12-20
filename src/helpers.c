
void to_lowercase(char *str, char *modified) {
  for (int i = 0; ; i++) {
    modified[i] = str[i];
    if (modified[i] == '\0') {
      break;
    }
    if (modified[i] == ' ') {
      modified[i] = '_';
    }
    else {
      modified[i] = tolower(modified[i]);
    }
  }
}