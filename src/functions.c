#include "../inc/mpm.h"

char	*strjoin(char *s1, char *s2) {
	char	*join;

	join = malloc(strlen(s1) + strlen(s2) + 1);
	if (join == NULL)
		return NULL;
	join = memcpy(join, s1, strlen(s1));
	memcpy(join + strlen(s1), s2, strlen(s2));
	join[strlen(s1) + strlen(s2)] = 0;
	return join;
}

static void	set_nb(char const *s, char c, int *nb) {
	int		i1, i2;

	i1 = i2 = 0;
	while (s[i1] != '\0') {
		if (i1 == i2 && s[i1] == c) {
			i1++;
			i2++;
		} else {
			if (s[i2] == c || s[i2] == '\0') {
				(*nb)++;
				i1 = i2;
			} else
				i2++;
		}
	}
}

static void	fill_tab(char const *s, char c, char **tab, int index) {
	int		i1, i2;
	i1 = i2 = 0;
	while (s[i1] != '\0') {
		if (i1 == i2 && s[i1] == c) {
			i1++;
			i2++;
		} else {
			if (s[i2] == c || s[i2] == '\0') {
				tab[index++] = ft_strsub(s, i1, i2 - i1);
				i1 = i2;
			} else
				i2++;
		}
	}
}

char	**ft_strsplit(char const *s, char c) {
	char	**tab;
	int		nb;

	tab = NULL;
	if (s) {
		nb = 0;
		set_nb(s, c, &nb);
		tab = (char **)malloc(sizeof(char *) * (nb + 1));
		if (tab) {
			tab[nb] = NULL;
			fill_tab(s, c, tab, 0);
		}
	}
	return (tab);
}

char	*ft_strsub(char const *s, unsigned int start, size_t len) {
	size_t	length;
	char	*out;

	length = (size_t)strlen((char *)s);
	out = NULL;
	if ((start + len) <= length) {
		out = (char *)malloc(sizeof(char) * (len + 1));
		if (out) {
			out = strncpy(out, s + start, len);
			out[len] = '\0';
		}
	}
	return (out);
}
