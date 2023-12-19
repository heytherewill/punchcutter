# 🖨️ Punchcutter

Punchcutter is a static website generator I've written for generating the files of [my personal website](github.com/heytherewill/heytherewill.github.io/).

# Usage

> [!NOTE]
> You can always call `punchcutter -h ` to see what parameters are required and their order.

Punchcutter will scan the provided directory for `.md` files and will generate one HTML file, based on the provided template, for each markdown file it finds.
It will output all files to the same directory, but the tree structure is preserved for the sake of building the navigation links. 

Punchcutter expects that inside every folder there should be a `.md` file with the same name (except for the root directory, where it expects there to be an `index.md`).

The currently supported replacements for the template are:

| Template key       | Value which will replace it                                                                     |
| ------------------ | ----------------------------------------------------------------------------------------------- |
| `{PAGE_CONTENT}`   | The contents of the markdown file, converted to HTML                                            |
| `{PAGE_LINKS}`     | The navigation structure for the page, including links to grandparent, parent and sibling pages |
| `{PAGE_[METADATA]}`| Any arbitrary key x in your YAML front matter will replace the equivalent `{PAGE_X}`            |

On top of generating these, punchcutter will also recursively copy every file inside the `includes` directory provided. This is useful for images, stylesheets, static pages and the like.

# Building

```
cmake .
make
```
