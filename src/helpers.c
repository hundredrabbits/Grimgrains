void safe_cp_to_lowercase(char *str, char *target, size_t tsize) {
  for (size_t i = 0; i < tsize; i++) {
    target[i] = str[i];
    if (target[i] == '\0') {
      break;
    }
    if (target[i] == ' ') {
      target[i] = '_';
    }
    else {
      target[i] = tolower(target[i]);
    }
  }
  target[tsize - 1] = '\0';
}

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
