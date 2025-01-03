#ifdef _WIN32
#define dllexp extern __declspec(dllexport)
#else
#define dllexp /*extern __declspec(dllexport)*/
#endif
#include <stdio.h>

/*関数宣言*/
dllexp char* rewrite(char *p, int l, int i, char d);
dllexp char* trim (char *p, int l, int i);
dllexp char* find(char *p, int l, char tgt);
dllexp char* split(char *p, int l, char dchar, int i);
dllexp char* slice(char *p, int l, int si, int ei);
dllexp char* swap(char *p, int l);
dllexp char* unique(char *p, int l);
dllexp void sort(int *p, int l);
dllexp void twss(int *p, int l);
dllexp int lhash(char *p, int l);
dllexp void msgtype(int tablenum);
/*2024/12/22/12:26:関数宣言部を作成。*/

char res_list[0xFFFF] = "";; /* 2024/12/20/15:55:引数で変数を指定する方式から
					   関数の返り値を使用する方式に変更するため、
					   変数をグローバルとして宣言。65535文字までなら何とか。 */

dllexp char* rewrite(char *p, int l, int i, char d)
{
	/* 2024/12/07/12:49:入力配列pを破壊しないように変更しました */
	
	for (int j = 0; j < l; j++)
	{
		res_list[j] = *(p + j); /* 2024/12/19/09:40:バグがあったので修正 */
		if(j == i)
		{
			res_list[j] = d;
		}
	}
	return res_list;
}

dllexp char* trim(char *p, int l, int i)
{
	/* 2024/12/07/12:49:入力配列pを破壊しないように変更しました */ 
	
	for (int j = 0; j < l; j++)
	{
		if(i <= j)
		{
			if (j == l - 1)
			{
				res_list[j] = '\0';
			}
			else
			{
				res_list[j] = *(p + j + 1);
			}
		}
		else
		{
			res_list[j] = *(p + j);
		}
	}
	return res_list;
}

dllexp char* find(char *p, int l, char tgt)
{
	int j = 0;
	for(int i = 0; i < l; i++)
	{
		if(*(p + i) == tgt)
		{
			res_list[j] = i;
			j++;
		}
	}
	return res_list;
}

/* 2024/12/06/21:24:old_splitは削除されました */

dllexp char* split(char *p, int l, char dchar, int i)
{
	int k = 0;
	int n = 0;
	 
	char tmp[l + 2];

	for(int ti = 0; ti < l; ti++)
	{
		tmp[ti + 1] = *(p + ti);
	}
	tmp[l + 1] = dchar;
	tmp[0] = dchar;

	for(int j = 0; j < l + 2; j++)
	{
		if(tmp[j] == dchar)
		{
			k++;
			if(k - 1 == i)
			{
				j++;
				while(1)
				{
					if(k - 1 == i + 1)
					{
						return res_list;
					}
					if(tmp[j] == dchar)
					{
						k++;
					}
					else
					{
						res_list[n] = tmp[j];
					}
					n++;
					j++;
				}
			}
		}
	}
	return res_list;
}

dllexp char* slice(char *p, int l, int si, int ei)
{
	char tmp[l + 2];

	for(int i = 0; i < l; i++)
	{
		tmp[i + 1] = *(p + i);
	}
	tmp[ei + 1] = '\0';
	tmp[si] = '\0';
	return split(&tmp[0], l, '\0', 1);
}

dllexp char* swap(char *p, int l)
{
	for(int i = 0; i < l; i++)
	{
		res_list[i] = *(p + l - i - 1);
	}
	return res_list;
}

dllexp char* unique(char *p, int l)
{
	int k = 0;

	for(int i = 0; i < l; i++)
	{
		/* 2024/12/06/21:21:修正完了。もともとのコードは消しました。 */
		/* n番目の要素と*(p + i)が一致したらflagを1にする、forする前に0にして置き、forし終わった後にflag==0ならres_list[k]に追加、k++をする */
	   int flag = 0;
		for(int n = 0; n < l; n++)
		{
			if(*(p + i) == res_list[n])
			{
				flag = 1;
				break; // AIの指摘により、「flagを立てたら、そのあとは調べるの無意味じゃない？」ということでforから抜けるように。
			}
		}

		if(flag == 0)
		{
			res_list[k] = *(p + i);
			k++;
		}
	}
	return res_list;
}

dllexp void sort(int *p, int l)
{
	twss(p, l);
	return;
}

dllexp void twss(int *p, int l)
{
	/* 2024/12/14/21:14:このアルゴリズムの名前が「双方向選択ソート」、
	英語では「Two Way Selection Sort」になったため、関数名をtwssとさせていただきました。
	なお、既存のプログラムでの利用でエラーが起きないよう、
	sort関数ではtwssを呼び出すよう変更しております。 */
	int tmp; // 値のコピー用
	int i = 0;
	int k = l - 1;

	while(1)
	{
		if(*(p + i) > *(p + k))
		{
			tmp = *(p + i);
			*(p + i) = *(p + k);
			*(p + k) = tmp;
		}
		k--;

		if(i == k)
		{
			i++;
			k = l - 1;
		}
		if(i == l - 1)
		{
			return;
		}
	}
}

dllexp int lhash(char *p, int l)
{
	/* 2024/12/19/21:18:実装。lはlikeのl。 */
	int res = 0;
	int i = 0;
	int tmp = 0;

	for(i = 0; i < l; i++)
	{
		tmp = res % ((int)*(p + i) * (i + 1) - tmp) + (int)*(p + i);  /* 2024/12/22/15:26:テストした結果順序が異なっても同じ値になってしまっていたため修正。 */
		res = res + tmp;
	}
	return res;
}

dllexp void msgtype(int tablenum)
{
	switch (tablenum)
	{
	case 0:
		printf("\e[40m \e[41m \e[42m \e[43m \e[44m \e[45m \e[46m \e[47m \e[0m");
		break;
	case 1:
		printf("\e[42m[\e[30m INFO ]\e[0m ");
		break;
	case 2:
		printf("\e[41m[\e[38m ERROR ]\e[0m ");
		break;
	case 3:
		printf("\e[43m[\e[30m WARNING ]\e[0m "); /* 2024/12/19/06:07:スペルミス発覚。どんなミスしてんねん。 */
		break;
	case 4:
		printf("\e[44m[\e[30m DEBUG ]\e[0m ");
		break;
	case 5:
		printf("\e[46m[\e[30m NOTE ]\e[0m ");
		break;
	}
	return;
}
