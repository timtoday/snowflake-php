<?php
$ids=array();
for(;;){
  $id=sid();
  print $id."\n";
  if(isset($ids[$id]))
  {
    print $id."repeat!!! \n";exit;
  }else{
      $ids[$id]=$id;
  }


} ?>