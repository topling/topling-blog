

博客仓库只关注两个目录：

1. 文章目录 `posts`

2. 资源目录 `source`

其他目录将被忽略！

---

关于文章目录 `posts`:

文章必须是 markdown 文本文件，必须以 `.md` 为后缀。

* 文章标题：markdown 文件的名字（`.md` 前的部分）。
* 文章分类：与文章目录的层级对应。
* 文章发表时间：第一次 commit 的时间。
* 文章更新时间：最后一次 commit 的时间。



例如，有文章 `posts/A/B/example.md`，则文章 `example.md` 的标题为 `example`，文章的分类为 `A->B`（即 `A` 类别的子类 `B`）。

---

关于资源目录 `source`：

每一篇文章都有一个独立的资源目录，资源目录与对应文章层级对应且同名。

例如 `posts/A/B/example.md`，其资源文件夹为 `source/A/B/example/`，资源文件夹的内容即为此文章的资源。

**注意：**暂时不支持资源目录中的子目录，请不要在资源目录里再创建目录，所有资源应该平铺在资源文件夹内。不论是图片、视频、还是代码文件等。

---

在文章内引用图片，使用绝对路径（GitHub 对于相对路径的处理似乎有些迷）

引用图片格式：

```
![图片描述](https://https://raw.githubusercontent.com/组织名/仓库名/分支/资源路径)
```



例如在 topling/topling-blog 仓库的 main 分支下， `posts/A/B/example.md` 有图片 `source/A/B/example/img.png`，则文章内的引用此图片的方法应为：

`![图片引用举例](https://https://raw.githubusercontent.com/topling/topling-blog/main/source/A/B/example/img.png)`

注意，如果链接中有空格，请用 %20 代替。







------

------

