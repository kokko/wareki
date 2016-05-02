<?php
/**
 *
 * 西暦→和暦変換
 *
 * @author      kokko
 * @version     1.0
 * @create      2016-05-03
 *
 **/

//元号データ 降順で設定すること
$ARR_GENGOU = array(
	'H'	=>	array(
		'name'	=>	'平成',
		'since'	=>	'1989-01-08',
		'to'	=>	false,
	),
	'S'	=>	array(
		'name'	=>	'昭和',
		'since'	=>	'1926-12-25',
		'to'	=>	'1989-01-07',
	),
	'T'	=>	array(
		'name'	=>	'大正',
		'since'	=>	'1912-07-30',
		'to'	=>	'1926-12-24',
	),
	'M'	=>	array(
		'name'	=>	'明治',
		'since'	=>	'1868-01-25',
		'to'	=>	'1912-07-29',
	),
);


class Wareki {

	//エラー格納領域
	var $errors;

	/**
	 *	西暦(yyyy-mm-dd) → 和暦(gyy.mm.dd)
	 *	$nihongo = true にすると日本語表記(例:平成28年5月3日)で返します。
	 */
	public static function ToWareki($seireki = null, $nihongo = false)
	{
		global $ARR_GENGOU;
	
		$ret = false;
	
		//西暦指定がない場合は今日の日付を使う
		if ( !isset($seireki) ) {
			$seireki = date('Y-m-d');
		}

		//西暦日付が正しい形式か？
		list($yy, $mm, $dd) = explode('-', $seireki);
		if ( checkdate($mm, $dd, $yy) ) {

			//元号データを降順で探索
			$now_gengou = '';
			foreach ( $ARR_GENGOU as $gengou_letter => $gengou_info ) {
				if ( (!$gengou_info['since'] || $gengou_info['since'] <= $seireki)
				&&   (!$gengou_info['to']    || $seireki <= $gengou_info['to']) ) {
					$now_gengou = $gengou_letter;
					break;
				}
			}
			//合致するものがあればその元号で日付作成
			if ( $now_gengou ) {

				list($yy_since, $mm_since, $dd_since) = explode('-', $ARR_GENGOU[$now_gengou]['since']);
				$date_year  = $yy - $yy_since + 1;
				$date_month = $mm;
				$date_day   = $dd;

				if ( !$nihongo ) {
					//英数字表記
					$ret = sprintf('%s%d.%d.%d', $now_gengou, $date_year, $date_month, $date_day);
				} else {
					//日本語表記
					$date_year_gen = ($date_year == 1)? '元': (string)$date_year;
					$ret = sprintf('%s%s年%d月%d日', $ARR_GENGOU[$now_gengou]['name'], $date_year_gen, $date_month, $date_day);
				}
				
			} else {
				$this->errors[] = '西暦から元号を特定できません';
			}

		} else {
			$this->errors[] = '日付が正しくありません';
		}
	
		return $ret;
	}
}

