var basePath;

function build_path()
{
    var r=document.getElementsByClassName("row")[0];
    var l=r.children.length;
    var relPath="";
    for(var i=0;i<l;i++)
    {
        var c=r.children[i];
        var buttons=c.children[0].children;
        for(var j=0;j<buttons.length;j++)
        {
            if(buttons[j].classList.contains("selected"))
            {
                if( buttons[j].hasAttribute("path") )
                {
                    relPath = relPath.concat(buttons[j].getAttribute("path"), "/");
                }
            }
        }
    }

    return relPath;
}

function get_current_display()
{
    var display = document.getElementsByClassName("display")[0];
    for(var i=0;i<display.children.length;++i)
    {
        if(display.children[i].classList.contains("active"))
        {
            return display.children[i];
        }
    }
}

function get_active_subdisplay(display_class)
{
    var activeDisplay = get_current_display();
    var subdisplays = activeDisplay.children;
    for(var i=0;i<subdisplays.length;++i)
    {
        if(subdisplays[i].classList.contains(display_class))
        {
            return subdisplays[i];
        }
    }
}

function load_figure()
{
    var relPath=build_path();

    var coco_annotation_img=basePath.concat("/data/",relPath,"coco-annotation.png");
    var coco_annotation = get_active_subdisplay("coco-annotation");
    coco_annotation.children[0].setAttribute("src",coco_annotation_img);

    var gc_seg_img = basePath.concat("/data/",relPath,"gc-seg.png");
    var gc_seg = get_active_subdisplay("gc-seg");
    gc_seg.children[0].setAttribute("src",gc_seg_img);

    var corrected_seg_with_data_img = basePath.concat("/data/",relPath,"corrected-seg-with-data.png");
    var corrected_seg_with_data = get_active_subdisplay("corrected-seg-with-data");
    corrected_seg_with_data.children[0].setAttribute("src",corrected_seg_with_data_img);

    var corrected_seg_without_data_img = basePath.concat("/data/",relPath,"corrected-seg-without-data.png");
    var corrected_seg_without_data = get_active_subdisplay("corrected-seg-without-data");
    corrected_seg_without_data.children[0].setAttribute("src",corrected_seg_without_data_img);
}


function select(el)
{
    _select(el,true);
}

function _select(el,load_flag)
{
    var list_li=el.parentElement.children;
    for (var i=0;i<list_li.length;i++)
    {
        if(list_li[i].hasAttribute("type"))
        {
            list_li[i].className="button";
        }

    }
    el.className="button selected";
    if(load_flag)
    {
        load_figure();
    }

}

function default_buttons()
{
    var r=document.getElementsByClassName("row")[0];
    var l=r.children.length;
    for(var i=0;i<l;i++)
    {
        var c=r.children[i];
        if(c.hasAttribute("menu"))
        {
            var buttons = c.children[0].children;
            var first = buttons[1];
            _select(first, true);
        }
    }
}

function open_src(el)
{
    document.location.href=el.getAttribute("src");
}

function init()
{
    //basePath=window.prompt("Enter images base folder");
    basePath=""
    if(basePath=="")
    {
        basePath=window.location.toString();
        basePath=basePath.substring(0, basePath.lastIndexOf("/") );
    }else
    {
        basePath="file://".concat(basePath);
    }
    default_buttons();
    load_figure();
    document.body.style.visibility="visible";

}


window.onload=init;